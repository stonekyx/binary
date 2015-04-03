#include <elf.h>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(reloc)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginRelocMainWindow", "Relocations"), plugin, param, parent),
    _infoModel(NULL)
{
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    QFont font = _ui->infoTree->font();
    font.setFamily("Courier");
    font.setPointSize(9);
    _ui->infoTree->setFont(font);
    updateInfo(_plugin->manager->getBackend()->getFile());
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 5, NULL);
    scanShdr(file);
    _ui->infoTree->setModel(_infoModel);
}

bool MainWindow::scanShdr(File *file)
{
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return false;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!file->getShdr(i, &shdr)) {
            continue;
        }
        if(shdr.sh_type != SHT_REL && shdr.sh_type != SHT_RELA) {
            continue;
        }
        _infoModel->buildMore(QString("Section\t%1")
                .arg(file->getScnName(&shdr)));
        for(size_t ent=0; ent<shdr.sh_size/shdr.sh_entsize; ent++) {
            if(shdr.sh_type == SHT_REL) {
                Elf64_Rel rel;
                if(!file->getRel(i, ent, &rel)) {
                    continue;
                }
            }
        }
    }
    return true;
}

END_PLUG_NAMESPACE
