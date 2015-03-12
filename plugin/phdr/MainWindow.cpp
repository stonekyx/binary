#include <QtCore/QTextStream>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(phdr)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginPhdrMainWindow", "Program header"), plugin, param, parent),
    _infoModel(NULL)
{
    updateInfo(_plugin->manager->getBackend()->getFile());
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateInfo(File *file)
{
    MWTreeView::updateInfo(file);

    QString modelData;
    QTextStream textStream(&modelData);
    textStream << "test" << "\t" << "testvalue";

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(modelData);
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
