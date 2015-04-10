#include <ctime>

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "backend/ConvertAddr.h"
#include "DemangleWrap.h"
#include "LoadWorker.h"

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(disasm)

LoadWorker::LoadWorker(File *file, InfoModel *im, QObject *parent) :
    QThread(parent), _file(file), _infoModel(im)
{
    _restricted = false;
    file->registerWatcher(this);
    QObject::connect(file, SIGNAL(aboutToBeDestroyed()),
            this, SLOT(terminate()));
}

LoadWorker::LoadWorker(size_t begin, size_t end,
        File *file, InfoModel *im, QObject *parent) :
    QThread(parent), _file(file), _infoModel(im), _begin(begin), _end(end)
{
    _restricted = true;
    file->registerWatcher(this);
    QObject::connect(file, SIGNAL(aboutToBeDestroyed()),
            this, SLOT(terminate()));
}

static QString getTooltip(const QString &orig)
{
    char *demangle = cplus_demangle(orig.toUtf8().constData());
    QString res(demangle);
    free(demangle);
    return res;
}

static QString getTooltip(Elf64_Addr addr, File::DisasmCBInfo *info)
{
    char *buf = info->convertAddr->vaddrToSecOffStrWithData(addr, 50);
    QString res(buf);
    free(buf);
    return res;
}

static QStringList splitArgs(const QString &args, char sep)
{
    QStringList res;
    int paran=0, bracket=0, brace=0, sharp=0;
    size_t last = 0;
    for(int i=0; i<args.length(); i++) {
        args[i] == '(' && (paran++);
        args[i] == ')' && (paran--);
        args[i] == '[' && (bracket++);
        args[i] == ']' && (bracket--);
        args[i] == '{' && (brace++);
        args[i] == '}' && (brace--);
        args[i] == '<' && (sharp++);
        args[i] == '>' && (sharp--);
        if(paran || bracket || brace || sharp) {
            continue;
        }
        if(args[i] == sep) {
            if(i-last) {
                res.push_back(args.mid(last, i-last));
            }
            last = i+1;
        } else if(i==args.length()-1) {
            if(i-last+1) {
                res.push_back(args.mid(last, i-last+1));
            }
        }
    }
    return res;
}

static bool sameSymbol(Elf64_Addr addr, File::DisasmCBInfo *info)
{
    Elf64_Sym lastSym;
    if(!info->file->getLastSymDataByFileOff(
            (char*)info->last - info->file->getRawData(0), &lastSym))
    {
        return false;
    }
    if(ELF64_ST_TYPE(lastSym.st_info) != STT_FUNC) { return false; }
    return lastSym.st_value + lastSym.st_size > info->vaddr &&
            addr >= lastSym.st_value &&
            addr < lastSym.st_value + lastSym.st_size;
}

QString LoadWorker::processBuffer(const char *buf, File::DisasmCBInfo *info)
{
    QStringList fields = QString(buf).split('\t', QString::SkipEmptyParts);
    QString comm;
    QStringList args, labels, tooltips;
    QMap<QString, QString> toolTipMap;
    if(fields.size()>0) comm = fields.at(0);
    if(fields.size()>1) args = splitArgs(fields.at(1), ',');
    if(fields.size()>2) labels = splitArgs(fields.at(2), ' ');
    if(labels.startsWith("#")) {
        labels.removeFirst();
    }
    for(int i=0; i<labels.size(); i++) {
        if(labels[i].startsWith('<') && labels[i].endsWith('>')) {
            labels[i] = labels[i].mid(1, labels[i].length()-2);
        }
        bool ok;
        Elf64_Addr addr = labels[i].toULong(&ok, 0);
        if(ok) {
            char *buf = info->convertAddr->vaddrToSecOffStrWithOrig(addr);
            if(buf) {
                labels[i] = buf;
                toolTipMap[labels[i]] = getTooltip(addr, info);
                free(buf);
            }
        }
    }
    foreach(QString arg, args) {
        bool ok;
        Elf64_Addr addr;
        if(arg.startsWith('$')) {
            addr = arg.right(arg.length()-1).toULong(&ok, 0);
        } else {
            addr = arg.toULong(&ok, 0);
        }
        if(!ok) continue;
        Elf64_Off fileOff;
        const char *symName;
        if(info->convertAddr->vaddrToFileOff(fileOff, addr) &&
                (symName = info->file->getSymNameByFileOff(fileOff)))
        {
            labels.push_back(symName);
        } else if(((LoadWorker*)info->data)->_ehdr.e_type != ET_REL &&
                !sameSymbol(addr, info))
        {
            char *buf = info->convertAddr->vaddrToSecOffStrWithOrig(addr);
            if(buf) {
                labels.push_back(buf);
                toolTipMap[labels.last()] = getTooltip(addr, info);
                free(buf);
            }
        }
    }
    const char *relocName;
    if((relocName = info->file->getRelocNameByFileOff(
                    (const char*)info->last - info->file->getRawData(0),
                    (const char*)info->cur - info->file->getRawData(0))))
    {
        labels.push_back(relocName);
    }
    foreach(QString label, labels) {
        if(toolTipMap.find(label) != toolTipMap.end()) {
            tooltips.push_back(toolTipMap[label]);
        } else {
            tooltips.push_back(getTooltip(label));
        }
    }
    if(relocName) {
        labels.last().prepend("Reloc: ");
    }
    QString res = comm;
    if(!args.isEmpty()) {
        res += "\t" + args.join(",");
    }
    if(!labels.isEmpty()) {
        res += "\t# <" + labels.join("> <") + ">";
        if(!tooltips.isEmpty()) {
            res += "\x1f" + tooltips.join("\n");
        }
    }
    return res;
}

int LoadWorker::disasmCallback(char *buf, size_t , void *arg)
{
    File::DisasmCBInfo *info = (File::DisasmCBInfo*)arg;
    LoadWorker *worker = (LoadWorker*)info->data;
    InfoModel *infoModel = worker->_infoModel;
    QString bytes;
    for(const uint8_t *p=info->last; p != info->cur; p++) {
        if(!bytes.isEmpty()) {
            bytes += " ";
        }
        bytes += QString("%1").arg(*p, 2, 16, QChar('0'));
    }
    const char *symName = info->file->getSymNameByFileOff(
            info->last - (const uint8_t*)info->file->getRawData(0));
    if(symName) {
        char *demangled = cplus_demangle(symName);
        QModelIndex inserted = infoModel->buildMore(
                QString("\t").repeated(worker->_instIndentLevel) +
                QString("%1\x1f%2")
                .arg(symName)
                .arg(demangled));
        worker->symbolStarted(inserted);
        free(demangled);
    }
    infoModel->buildMore(QString("\t").repeated(worker->_instIndentLevel)+
            QString("0x%1\t%2\t%3")
            .arg(info->vaddr, 0, 16)
            .arg(bytes)
            .arg(processBuffer(buf, info)));
    worker->_noSleep += info->cur - info->last;
    info->vaddr += info->cur - info->last;
    info->last = info->cur;
    if(info->labelBuf) {
        info->labelBuf[0] = 0;
    }
    if(worker->_noSleep > 0x1000) {
        msleep(100);
        worker->_noSleep = 0;
    }
    worker->_instCnt++;
    return 0;
}

void LoadWorker::stopTimer()
{
    struct timespec nowtime;
    clock_gettime(CLOCK_MONOTONIC, &nowtime);
    double start = _startTime.tv_sec + (double)_startTime.tv_nsec*1e-9;
    double end = nowtime.tv_sec + (double)nowtime.tv_nsec*1e-9;
    emit timerStopped(end-start, (end-start)/_instCnt);
}

void LoadWorker::run()
{
    clock_gettime(CLOCK_MONOTONIC, &_startTime);
    QObject::connect(this, SIGNAL(finished()),
            this, SLOT(stopTimer()));
    QObject::connect(this, SIGNAL(terminated()),
            this, SLOT(stopTimer()));
    _noSleep = 0;
    _instCnt = 0;
    if(!_file->getEhdr(&_ehdr)) {
        return;
    }
    if(_restricted) {
        _instIndentLevel = 0;
        _file->disasm(_begin, _end, disasmCallback, this);
    } else {
        _instIndentLevel = 1;
        runAll();
    }
}

void LoadWorker::runAll()
{
    size_t shdrNum;
    if(_file->getShdrNum(&shdrNum) != 0) {
        return;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!_file->getShdr(i, &shdr)) {
            continue;
        }
        if(shdr.sh_type != SHT_PROGBITS || shdr.sh_size == 0 ||
                (shdr.sh_flags & SHF_EXECINSTR) == 0)
        {
            continue;
        }
        QModelIndex ins = _infoModel->buildMore(QString("Section   %1")
                .arg(_file->getScnName(&shdr)));
        emit symbolStarted(ins);
        _file->disasm(shdr.sh_offset, shdr.sh_offset+shdr.sh_size,
                disasmCallback, this);
    }
}

END_PLUG_NAMESPACE
