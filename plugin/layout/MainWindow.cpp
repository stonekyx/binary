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
    updateInfo(_plugin->manager->getBackend()->getFile());
}

MainWindow::~MainWindow()
{ }

void MainWindow::updateInfo(File *file)
{
    _ui->scnLayout->updateInfo(file);
    _ui->sgmLayout->updateInfo(file);
    if(!_ui->switchMode(file)) {
        return;
    }
}

END_PLUG_NAMESPACE
