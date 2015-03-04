#include "mainwindow.h"
#include <QApplication>

USE_BIN_NAMESPACE(frontend);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
