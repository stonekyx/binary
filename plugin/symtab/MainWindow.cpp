#include <elf.h>
#include <QtGui/QMessageBox>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"
#include "DemangleWrap.h"
#include "Defines.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(symtab)

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

static size_t symTabIndex(File *file, size_t knownIdx, Elf64_Shdr *dst)
{
    if(knownIdx != 0) {
        if(!file->getShdr(knownIdx, dst) ||
                (dst->sh_type != SHT_SYMTAB &&
                 dst->sh_type != SHT_DYNSYM))
        {
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
        if(dst->sh_type == SHT_SYMTAB) {
            return i;
        }
    }
    return 0;
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    switch(file->getKind()) {
    case File::KIND_AR:
        updateArInfo(file);
        break;
    case File::KIND_ELF:
        updateElfInfo(file);
        break;
    default:
        break;
    }
}

void MainWindow::updateArInfo(File *file)
{
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);

    size_t arsymNum = file->getArsymNum();
    for(size_t i=0; i<arsymNum; i++) {
        Arsym arsym = file->getArsym(i);
        _infoModel->buildMore(QString("Entry %1\t%2")
                .arg(i)
                .arg(arsym.name()));
        QString objName;
        Arhdr arhdr = file->getArhdrByOffset(arsym.off());
        if(arhdr.name()) {
            objName = QString(" (%1)").arg(arhdr.name());
        }
        _infoModel->buildMore(QString("\tOffset\t0x%1%2")
                .arg(arsym.off(), 0, 16)
                .arg(objName));
        _infoModel->buildMore(QString("\tHash\t0x%1")
                .arg(arsym.hash(), 0, 16));
    }

    _ui->infoTree->setModel(_infoModel);
}

void MainWindow::updateElfInfo(File *file)
{
    Elf64_Shdr shdr;
    _scnIndex = symTabIndex(file, _scnIndex, &shdr);
    if(_scnIndex == 0) {
        QMessageBox::critical(this, tr("Error"),
                tr("No symbol table found or not a valid symbol table."));
        close();
        return;
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);

    size_t symCount = shdr.sh_size / shdr.sh_entsize;
    for(size_t i=0; i<symCount; i++) {
        Elf64_Sym sym;
        if(!file->getSym(_scnIndex, i, &sym)) {
            continue;
        }

        _infoModel->buildMore(QString("Entry %1\t%2")
                .arg(i)
                .arg(file->getStrPtr(shdr.sh_link, sym.st_name)));
        char *demangle = cplus_demangle(file->getStrPtr(shdr.sh_link, sym.st_name));
        _infoModel->buildMore(QString("\tDemangle\t%1")
                .arg(demangle));
        free(demangle);
        _infoModel->buildMore(QString("\tName offset\t0x%1")
                .arg(sym.st_name));
        _infoModel->buildMore(QString("\tInfo\t0x%1")
                .arg(sym.st_info, 0, 16));
        _infoModel->buildMore(QString("\t\tBind\t%1")
                .arg(Defines::commentText_STB(ELF64_ST_BIND(sym.st_info))));
        _infoModel->buildMore(QString("\t\tType\t%1")
                .arg(Defines::commentText_STT(ELF64_ST_TYPE(sym.st_info))));
        _infoModel->buildMore(QString("\tVisibility\t%1")
                .arg(Defines::commentText_STV(ELF64_ST_VISIBILITY(sym.st_other))));
        Elf64_Shdr shdr;
        QString scnName;
        if(sym.st_shndx && file->getShdr(sym.st_shndx, &shdr) &&
                !(scnName=file->getScnName(&shdr)).isEmpty())
        {
            scnName.prepend(" (");
            scnName.append(")");
        }
        _infoModel->buildMore(QString("\tSection index\t%1%2")
                .arg(sym.st_shndx).arg(scnName));
        _infoModel->buildMore(QString("\tValue\t%1")
                .arg(sym.st_value, 0, 16));
        _infoModel->buildMore(QString("\tSize\t%1")
                .arg(sym.st_size, 0, 16));
    }
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
