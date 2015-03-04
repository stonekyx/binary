#include <QtGui/QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

BEGIN_BIN_NAMESPACE(frontend)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
            "", tr("ELF files (*)"));
    if(!fileName.isEmpty()) {
        ui->labelHome->setText(fileName);
    }
}

END_BIN_NAMESPACE
