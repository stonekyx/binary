#include "mainwindow.h"
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QtGui/QCleanlooksStyle>

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
        if(args.at(i) == QString::fromUtf8("--file") &&
                i+1 < args.size())
        {
            w.openFile(args.at(i+1));
        }
    }
    QApplication::setStyle(new QCleanlooksStyle());
    w.show();

    return a.exec();
}
