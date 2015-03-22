#include <QtCore/QTextStream>
#include <QtGui/QMenu>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"
#include "Defines.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(shdr)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginShdrMainWindow", "Section header"), plugin, param, parent),
    _infoModel(NULL)
{
    ctxMenuTreeView();
    updateInfo(_plugin->manager->getBackend()->getFile());
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

#define HEX(val) (QString("0x%1").arg(val, 0, 16))
#define DEC(val) (QString("%1").arg(val))

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }

    QString modelData;
    QTextStream textStream(&modelData);

    Elf64_Shdr shdr;
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return;
    }

    for(size_t i=0; i<shdrNum; i++) {
        if(!file->getShdr(i, &shdr)) {
            continue;
        }
        if(!modelData.isEmpty()) {
            textStream << "\n";
        }
        const char *name = file->getScnName(&shdr);
        textStream << "Entry " << i << "\t";
        if(name && strlen(name)) {
            textStream << name;
        } else {
            textStream << "(No name)";
        }

        textStream << "\n\tName index\t" << DEC(shdr.sh_name);
        textStream << "\n\tType\t" << Defines::commentText_SHT(shdr.sh_type);
        textStream << "\n\tFlags\t" << HEX(shdr.sh_flags);
        vector<const char *> flagsName = Defines::macroText_SHF(shdr.sh_flags);
        vector<const char *> flagsText = Defines::commentText_SHF(shdr.sh_flags);
        for(size_t i=0; i<flagsName.size(); i++)
        {
            textStream << "\n\t\t" << flagsName[i] << "\t" << flagsText[i];
        }

        textStream << "\n\tVaddr at execution\t" << HEX(shdr.sh_addr);
        textStream << "\n\tFile offset\t" << HEX(shdr.sh_offset);
        textStream << "\n\tSize\t" << HEX(shdr.sh_size);
        textStream << "\n\tLink\t" << DEC(shdr.sh_link);
        textStream << "\n\tInfo\t" << HEX(shdr.sh_info);
        textStream << "\n\tSection alignment\t" << HEX(shdr.sh_addralign);
        textStream << "\n\t(sub)Entry size\t" << HEX(shdr.sh_entsize);
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(modelData, 2);
    _ui->infoTree->setModel(_infoModel);
}

void MainWindow::ctxMenuTreeView()
{
    _ctxActionMapper = new QSignalMapper(_ui->ctxMenu);

    QAction *actionShowData = _ui->ctxMenu->addAction(
            tr("Show section data"),
            _ctxActionMapper, SLOT(map()));
    actionShowData->setParent(_ui->ctxMenu);
    _ctxActionMapper->setMapping(actionShowData, "ScnData");

    QAction *actionShowStrTab = _ui->ctxMenu->addAction(
            tr("Show as string table"),
            _ctxActionMapper, SLOT(map()));
    actionShowStrTab->setParent(_ui->ctxMenu);
    _ctxActionMapper->setMapping(actionShowStrTab, "StrTab");

    QAction *actionShowSymTab = _ui->ctxMenu->addAction(
            tr("Show as symbol table"),
            _ctxActionMapper, SLOT(map()));
    actionShowSymTab->setParent(_ui->ctxMenu);
    _ctxActionMapper->setMapping(actionShowSymTab, "SymTab");

    QObject::connect(_ctxActionMapper, SIGNAL(mapped(const QString &)),
            this, SLOT(showSection(const QString &)));
}

void MainWindow::showSection(const QString &pluginName)
{
    QAction *action = dynamic_cast<QAction*>(
            _ctxActionMapper->mapping(pluginName));
    if(!action) {
        return;
    }
    QModelIndex index = _ui->infoTree->indexAt(action->data().toPoint());
    map<string, string> param;
    param["scnIndex"] = QString::number(index.row()).toUtf8().constData();
    BIN_NAMESPACE(frontend)::Plugin *plugin =
        _plugin->manager->getPlugin(pluginName.toUtf8().constData());
    if(plugin) {
        plugin->createView(param);
    }
}

#undef HEX

END_PLUG_NAMESPACE
