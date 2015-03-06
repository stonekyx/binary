#include "ui_MainWindow.h"

#include "MainWindow.h"

BEGIN_PLUG_NAMESPACE(basic)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
}

END_PLUG_NAMESPACE
