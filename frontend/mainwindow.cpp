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
    _backend(new Backend),
    _pm(new PluginManager(_backend))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _pm;
    delete _backend;
}

void MainWindow::loadPlugins(const QString &dirPath)
{
    QDir dir(dirPath);
    QStringList nameFilters;
    nameFilters << "*.so";
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::Files);
    foreach(QFileInfo f, files) {
        loadPlugin(f.absoluteFilePath());
    }
}

void MainWindow::loadPlugin(const QString &path)
{
    if(path.isNull()) {
        return;
    }
    QByteArray pathBytes = path.toUtf8();
    int idx = _pm->loadPlugin(pathBytes.constData());
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

void MainWindow::openFile(const QString &path)
{
    if(!path.isEmpty()) {
        ui->labelHome->setText(path);
        _backend->openFile(path.toUtf8().constData());
    }
}

void MainWindow::openFile()
{
    openFile(QFileDialog::getOpenFileName(this, tr("Open file"),
            "", tr("ELF files (*)")));
}

void MainWindow::closeFile()
{
    _backend->closeFile();
}

void MainWindow::loadPlugin()
{
    loadPlugin(QFileDialog::getOpenFileName(this, tr("Open plugin"),
            QString(), tr("Shared object (*.so)")));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Q:
        if(event->modifiers() & Qt::ControlModifier)
            QApplication::quit();
        break;
    default:
        break;
    }
}

END_BIN_NAMESPACE
