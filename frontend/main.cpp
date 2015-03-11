#include "mainwindow.h"
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QApplication>

USE_BIN_NAMESPACE(frontend);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QStringList args = QCoreApplication::arguments();
    for(int i=1; i<args.size(); i++) {
        if(args.at(i) == QString::fromUtf8("--plugins") &&
                i+1 < args.size())
        {
            w.loadPlugins(args.at(i+1));
        }
    }
    w.show();

    return a.exec();
}
