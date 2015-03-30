#include <elf.h>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(disasm)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginDisasmMainWindow", "Disassemble"), plugin, param, parent),
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

static int disasmCallback(char *buf, size_t , void *arg)
{
    File::DisasmCBInfo *info = (File::DisasmCBInfo*)arg;
    InfoModel *infoModel = (InfoModel*)info->data;
    infoModel->buildMore(QString("\t%1").arg(buf));
    return 0;
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 3, NULL);
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!file->getShdr(i, &shdr)) {
            continue;
        }
        if(shdr.sh_type != SHT_PROGBITS || shdr.sh_size == 0 ||
                (shdr.sh_flags & SHF_EXECINSTR) == 0)
        {
            continue;
        }
        _infoModel->buildMore(QString("Section\t%1")
                .arg(file->getScnName(&shdr)));
        file->disasm(i, disasmCallback, _infoModel);
    }
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
