#include "ui_MainWindow.h"

#include "MainWindow.h"

using namespace std;

BEGIN_PLUG_NAMESPACE(basic)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow())
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    show();
}

MainWindow::MainWindow(map<string, string> param, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow())
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

END_PLUG_NAMESPACE
