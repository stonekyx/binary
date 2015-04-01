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
    InfoModel *infoModel = (InfoModel*)info->data;
    QString bytes;
    for(const uint8_t *p=info->last; p != info->cur; p++) {
        if(!bytes.isEmpty()) {
            bytes += " ";
        }
        bytes += QString("%1").arg(*p, 2, 16, QChar('0'));
    }
    infoModel->buildMore(QString("\t0x%1\t%2\t%3")
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
        _infoModel->buildMore(QString("Section\t%1")
                .arg(_file->getScnName(&shdr)));
        _file->disasm(i, disasmCallback, _infoModel);
    }
}

END_PLUG_NAMESPACE
