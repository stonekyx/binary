#include <iostream>
#include <elf.h>
#include <QKeyEvent>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"

#include "ui_MainWindow.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(basic)

static QString modelData =
"Class	\002INFO_FIELD_CLASS\003";

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param,
        QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow()),
    _plugin(plugin),
    _infoModel(new InfoModel(modelData))
{
    ui->setupUi(this);
    ui->infoTree->setModel(_infoModel);
    QObject::connect(_plugin->manager->getBackend(),
            SIGNAL(fileChanged(binary::backend::File *)),
            this, SLOT(updateInfo(binary::backend::File *)));
    updateInfo(_plugin->manager->getBackend()->getFile());
    setAttribute(Qt::WA_DeleteOnClose);
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _infoModel;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_W:
        if(event->modifiers() & Qt::ControlModifier)
            close();
        break;
    default:
        break;
    }
}

void MainWindow::updateInfo(File *file)
{
    if(!file) {
        ui->infoTree->hide();
        ui->defaultLabel->show();
        return;
    }
    ui->defaultLabel->hide();
    ui->infoTree->show();

    int elfClass = file->getClass();
    if(elfClass == ELFCLASS32) {
        _infoModel->setData("INFO_FIELD_CLASS", QApplication::translate("PluginBasicMainWindow", "32 bit", 0, QApplication::UnicodeUTF8));
    } else if(elfClass == ELFCLASS64) {
        _infoModel->setData("INFO_FIELD_CLASS", QApplication::translate("PluginBasicMainWindow", "64 bit", 0, QApplication::UnicodeUTF8));
    } else {
        _infoModel->setData("INFO_FIELD_CLASS", QApplication::translate("PluginBasicMainWindow", "Unknown", 0, QApplication::UnicodeUTF8));
    }
}

END_PLUG_NAMESPACE
