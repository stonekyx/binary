#include <elf.h>
#include <QtGui/QMessageBox>

#include "backend/File.h"
#include "backend/ConvertClass.h"
#include "ui_MWTreeView.h"
#include "DemangleWrap.h"
#include "Defines.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(dyn)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginDynMainWindow", "Dynamic linking info"), plugin, param, parent),
    _infoModel(NULL)
{
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    updateInfo();
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

static size_t dynIndex(File *file, size_t knownIdx, Elf64_Shdr *dst)
{
    if(knownIdx != 0) {
        if(!file->getShdr(knownIdx, dst) || dst->sh_type != SHT_DYNAMIC) {
            return 0;
        }
        return knownIdx;
    }
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return 0;
    }
    for(size_t i=0; i<shdrNum; i++) {
        if(!file->getShdr(i, dst)) {
            continue;
        }
        if(dst->sh_type == SHT_DYNAMIC) {
            return i;
        }
    }
    return 0;
}

static char *dynPhdr(File *file, size_t *size)
{
    size_t phdrNum;
    if(file->getPhdrNum(&phdrNum) != 0) {
        return NULL;
    }
    for(size_t i=0; i<phdrNum; i++) {
        Elf64_Phdr phdr;
        if(!file->getPhdr(i, &phdr)) {
            continue;
        }
        if(phdr.p_type == PT_DYNAMIC) {
            if(size) {
                *size = phdr.p_filesz;
            }
            return file->getRawData(phdr.p_offset);
        }
    }
    return NULL;
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        _scnIndex = 0;
        return;
    }
    bool useShdr = false;
    bool usePhdr = false;
    char *dynRaw=NULL, *dynStrRaw=NULL;
    size_t dynRawSize;
    Elf64_Shdr shdr;
    _scnIndex = dynIndex(file, _scnIndex, &shdr);
    if(_scnIndex) {
        useShdr = true;
    } else if((dynRaw = dynPhdr(file, &dynRawSize))) {
        usePhdr = true;
        dynStrRaw = file->findDynTag(DT_STRTAB);
    }
    if(!useShdr && !usePhdr) {
        QMessageBox::critical(this, tr("Error"),
                tr("No dynamic section found!"));
        close();
        return;
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);

    size_t entCount;
    size_t entSize = file->getClass()==ELFCLASS32 ?
        sizeof(Elf32_Dyn) : sizeof(Elf64_Dyn);
    if(useShdr) {
        entCount = shdr.sh_size / shdr.sh_entsize;
    } else {
        entCount = dynRawSize/entSize;
    }
    for(size_t i=0; i<entCount; i++) {
        Elf64_Dyn dyn;
        if(useShdr) {
            if(!file->getDyn(_scnIndex, i, &dyn)) {
                continue;
            }
        } else {
            if(entSize == sizeof(Elf32_Dyn)) {
                ConvertClass::cvt(dyn, *(Elf32_Dyn*)(dynRaw+i*entSize));
            } else {
                ConvertClass::cvt(dyn, *(Elf64_Dyn*)(dynRaw+i*entSize));
            }
        }

        _infoModel->buildMore(QString("Entry %1\t%2")
                .arg(i)
                .arg(Defines::macroText(dyn.d_tag, defines_DT)));
        _infoModel->buildMore(QString("\tType\t%1")
                .arg(Defines::commentText(dyn.d_tag, defines_DT)));
        _infoModel->buildMore(QString("\td_un\t"));
        _infoModel->buildMore(QString("\t\td_val\t%1")
                .arg(dyn.d_un.d_val));
        _infoModel->buildMore(QString("\t\td_ptr\t0x%1")
                .arg(dyn.d_un.d_ptr, 0, 16));
        if(dyn.d_tag == DT_NEEDED || dyn.d_tag == DT_SONAME ||
                dyn.d_tag == DT_RPATH || dyn.d_tag == DT_RUNPATH) {
            _infoModel->buildMore(QString("\tNeeded library\t%1")
                    .arg(useShdr ?
                        file->getStrPtr(shdr.sh_link, dyn.d_un.d_val) :
                        (dynStrRaw ? dynStrRaw + dyn.d_un.d_val : NULL)));
        }
    }
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
