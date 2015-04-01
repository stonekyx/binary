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
    _infoModel(NULL),
    _loadWorker(NULL)
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
    if(_loadWorker) {
        _loadWorker->terminate();
        _loadWorker->wait(1000);
        delete _loadWorker;
    }
    if(_infoModel) {
        delete _infoModel;
    }
}

void MainWindow::updateInfo(File *file)
{
    if(_loadWorker)
    {
        _loadWorker->terminate();
        _loadWorker->wait(1000);
        delete _loadWorker;
        _loadWorker = NULL;
    }
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 5, NULL);
    _ui->infoTree->setModel(_infoModel);

    _loadWorker = new LoadWorker(file, _infoModel);
    QObject::connect(_loadWorker, SIGNAL(symbolStarted(QModelIndex)),
            this, SLOT(spanFirstColumn(QModelIndex)));
    _loadWorker->start();
}

void MainWindow::spanFirstColumn(QModelIndex index)
{
    _ui->infoTree->setFirstColumnSpanned(index.row(), index.parent(),
            true);
}

END_PLUG_NAMESPACE
