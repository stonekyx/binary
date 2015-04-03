#include <elf.h>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"
#include "DemangleWrap.h"

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
    _infoModel = new InfoModel(QString(), 2, NULL);
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
            union {
                Elf64_Rel rel;
                Elf64_Rela rela;
            } rel;
            if(shdr.sh_type == SHT_REL) {
                if(!file->getRel(i, ent, &rel.rel)) {
                    continue;
                }
            } else if(shdr.sh_type == SHT_RELA) {
                if(!file->getRela(i, ent, &rel.rela)) {
                    continue;
                }
            } else {
                continue;
            }
            Elf64_Sym sym;
            if(!file->getSym(shdr.sh_link,
                        ELF64_R_SYM(rel.rel.r_info), &sym))
            {
                continue;
            }
            Elf64_Shdr symShdr;
            if(!file->getShdr(shdr.sh_link, &symShdr)) {
                continue;
            }
            const char *symName =
                file->getStrPtr(symShdr.sh_link, sym.st_name);
            char *demangled = cplus_demangle(symName);
            _infoModel->buildMore(QString("\tSymbol\t%1\x1f%2")
                    .arg(symName)
                    .arg(demangled));
            _infoModel->buildMore(QString("\t\tOffset\t0x%1")
                    .arg(rel.rel.r_offset, 0, 16));
            _infoModel->buildMore(QString("\t\tType\t%1")
                    .arg(ELF64_R_TYPE(rel.rel.r_info)));
            if(shdr.sh_type == SHT_RELA) {
                _infoModel->buildMore(QString("\t\tAddend\t%1")
                        .arg(rel.rela.r_addend));
            }
            _infoModel->buildMore(QString("\t\tLibrary\t%1")
                    .arg(file->queryDynSymDeps(symName, NULL)));
            free(demangled);
        }
    }
    return true;
}

END_PLUG_NAMESPACE
