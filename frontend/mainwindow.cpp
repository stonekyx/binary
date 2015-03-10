#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtGui/QWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"

USE_BIN_NAMESPACE(backend);

BEGIN_BIN_NAMESPACE(frontend)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _pm(new PluginManager),
    _backend(new Backend)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _pm;
    delete _backend;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
            "", tr("ELF files (*)"));
    if(!fileName.isEmpty()) {
        ui->labelHome->setText(fileName);
        _backend->openFile(fileName.toUtf8().constData());
    }
}

void MainWindow::loadPlugin()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open plugin"),
            QString(), tr("Shared object (*.so)"));
    if(fileName.isNull()) {
        return;
    }
    QByteArray fileNameBytes = fileName.toUtf8();
    int idx = _pm->loadPlugin(fileNameBytes.constData());
    if(idx==-1) {
        QMessageBox::warning(this, tr("Error"),
                tr("Error when loading or plugin has been loaded before."));
        return;
    }
    QAction *pluginAction = _pm->getAction(idx, this);
    if(!pluginAction) {
        QMessageBox::warning(this, tr("Error"),
                tr("Error occurred in plugin."));
        return;
    }
    ui->pluginActions.push_back(pluginAction);
    ui->menuPlugin->addAction(pluginAction);
}

END_BIN_NAMESPACE
