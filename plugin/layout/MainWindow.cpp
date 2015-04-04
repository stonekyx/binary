#include <map>
#include <string>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;
USE_BIN_NAMESPACE(frontend);
USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(layout)

MainWindow::MainWindow(Plugin *plugin, map<string, string> param,
        QWidget *parent) :
    MWBase(new Ui::MainWindow(), plugin, param, parent),
    _ui(dynamic_cast<Ui::MainWindow*>(MWBase::_ui))
{
    QObject::connect(_ui->scnLayout, SIGNAL(openScn(size_t)),
            this, SLOT(openScn(size_t)));
    updateInfo();
}

MainWindow::~MainWindow()
{ }

void MainWindow::updateInfo(File *file)
{
    _ui->fileLayout->updateInfo(file);
    _ui->scnLayout->updateInfo(file);
    _ui->sgmLayout->updateInfo(file);
    if(!_ui->switchMode(file)) {
        return;
    }
}

void MainWindow::openScn(size_t scnIdx)
{
    File *file = _plugin->manager->getBackend()->getFile();
    Elf64_Shdr shdr;
    if(!file->getShdr(scnIdx, &shdr)) {
        return;
    }
    map<string, string> param;
    param["scnIndex"] = QString::number(scnIdx).toUtf8().constData();
    Plugin *openPlugin = NULL;
    switch(shdr.sh_type) {
#ifdef SHT_SYMTAB
    case SHT_SYMTAB:
#endif
#ifdef SHT_DYNSYM
    case SHT_DYNSYM:
#endif
#if defined(SHT_SYMTAB) || defined(SHT_DYNSYM)
        openPlugin = _plugin->manager->getPlugin("SymTab");
        break;
#endif

#ifdef SHT_STRTAB
    case SHT_STRTAB:
        openPlugin = _plugin->manager->getPlugin("StrTab");
        break;
#endif

#ifdef SHT_DYNAMIC
    case SHT_DYNAMIC:
        openPlugin = _plugin->manager->getPlugin("Dyn");
        break;
#endif

    default:
        if((shdr.sh_flags & SHF_STRINGS)) {
            openPlugin = _plugin->manager->getPlugin("StrTab");
            break;
        }
        openPlugin = _plugin->manager->getPlugin("ScnData");
    }
    if(openPlugin) {
        openPlugin->createView(param);
    }
}

END_PLUG_NAMESPACE
