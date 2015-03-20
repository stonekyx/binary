#include <elf.h>
#include <QtGui/QMessageBox>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"
#include "DemangleWrap.h"

#include "MainWindow.h"
#include "TypeDefines.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(dyn)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginSymTabMainWindow", "Symbol table"), plugin, param, parent),
    _infoModel(NULL)
{
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    updateInfo(_plugin->manager->getBackend()->getFile());
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

#define C(constant, text) \
    case (constant):return (text)

template<typename T, typename U>
bool rangeCheck(T var, U lo, U hi) {
    return var>=(T)lo && var<=(T)hi;
}

#define R(var, lo, hi, value) \
    do { if(rangeCheck(var, lo, hi)) {return (value);} }while(0)

static const char *typeNameText(Elf64_Sxword tag)
{
    if(typeDefines.defineMap.find(tag) != typeDefines.defineMap.end()) {
        return typeDefines.defineMap[tag].name;
    }
    DefineInfo &info = typeDefines.queryRange(tag);
    if(info.valid) {
        return info.name;
    }
    return "Unknown";
}

static const char *typeText(Elf64_Sxword tag)
{
    if(typeDefines.defineMap.find(tag) != typeDefines.defineMap.end()) {
        return typeDefines.defineMap[tag].explain;
    }
    DefineInfo &info = typeDefines.queryRange(tag);
    if(info.valid) {
        return info.explain;
    }
    return "Unknown";
}

#undef R
#undef C

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    Elf64_Shdr shdr;
    _scnIndex = dynIndex(file, _scnIndex, &shdr);
    if(_scnIndex == 0) {
        QMessageBox::critical(this, tr("Error"),
                tr("No dynamic section found!"));
        close();
        return;
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);

    size_t entCount = shdr.sh_size / shdr.sh_entsize;
    for(size_t i=0; i<entCount; i++) {
        Elf64_Dyn dyn;
        if(!file->getDyn(_scnIndex, i, &dyn)) {
            continue;
        }

        _infoModel->buildMore(QString("Entry %1\t%2")
                .arg(i)
                .arg(typeNameText(dyn.d_tag)));
        _infoModel->buildMore(QString("\tType\t%1")
                .arg(typeText(dyn.d_tag)));
        _infoModel->buildMore(QString("\td_un\t"));
        _infoModel->buildMore(QString("\t\td_val\t%1")
                .arg(dyn.d_un.d_val));
        _infoModel->buildMore(QString("\t\td_ptr\t0x%1")
                .arg(dyn.d_un.d_ptr, 0, 16));
        if(dyn.d_tag == DT_NEEDED || dyn.d_tag == DT_SONAME ||
                dyn.d_tag == DT_RPATH || dyn.d_tag == DT_RUNPATH) {
            _infoModel->buildMore(QString("\tNeeded library\t%1")
                    .arg(file->getStrPtr(shdr.sh_link, dyn.d_un.d_val)));
        }
    }
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
