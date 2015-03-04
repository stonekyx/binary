#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtGui/QWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"

BEGIN_BIN_NAMESPACE(frontend)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _pm(new PluginManager)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _pm;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
            "", tr("ELF files (*)"));
    if(!fileName.isEmpty()) {
        ui->labelHome->setText(fileName);
    }
}

void MainWindow::loadPlugin()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open plugin"),
            "*.so", tr("Shared object (*.so)"));
    QByteArray fileNameBytes = fileName.toUtf8();
    int idx = _pm->loadPlugin(fileNameBytes.constData());
    if(idx==-1) {
        QMessageBox::warning(this, tr("Error"),
                tr("Error when loading or plugin has been loaded before."));
        return;
    }
    void *sym = _pm->findSymbol(idx, "createAction");
    if(!sym) {
        QMessageBox::warning(this, tr("Error"),
                tr("Error when initializing plugin."));
        return;
    }
    typedef QAction *(*PluginActionCreator)(QWidget*);
    QAction *pluginAction = ((PluginActionCreator)sym) (this);
    if(!pluginAction) {
        QMessageBox::warning(this, tr("Error"),
                tr("Error occurred in plugin."));
        return;
    }
    ui->pluginActions.push_back(pluginAction);
    ui->menuPlugin->addAction(pluginAction);
}

END_BIN_NAMESPACE
