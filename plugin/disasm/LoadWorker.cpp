#include <QtCore/QString>
#include <QtCore/QStringList>

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

static QString addTooltip(const char *buf)
{
    QString res(buf);
    QStringList fields = res.split("\t", QString::SkipEmptyParts);
    if(fields.size() != 3) {
        return res;
    }
    QStringList units = fields[2].split(" ", QString::SkipEmptyParts);
    QString tooltip;
    foreach(QString u, units) {
        if(u.length()>2 &&
                u.startsWith(QChar('<')) && u.endsWith(QChar('>')))
        {
            if(!tooltip.isEmpty()) {
                tooltip += "\n";
            }
            char *demangle = cplus_demangle(
                    u.mid(1, u.length()-2).toUtf8().constData());
            tooltip += demangle;
            free(demangle);
        }
    }
    if(!tooltip.isEmpty()) {
        res += '\x1f';
        res += tooltip;
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
    const char *symName = info->file->getSymNameByVal(info->vaddr);
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
            .arg(addTooltip(buf)));
    info->vaddr += info->cur - info->last;
    info->last = info->cur;
    if(info->labelBuf) {
        info->labelBuf[0] = 0;
    }
    yieldCurrentThread();
    return 0;
}

void LoadWorker::run()
{
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
