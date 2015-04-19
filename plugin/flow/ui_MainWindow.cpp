#include "ui_MainWindow.h"

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(flow)
namespace Ui {

MainWindow::MainWindow() :
    MWBase("PluginFlowMainWindow", "Flow Diagram")
{}

MainWindow::~MainWindow()
{
}

bool MainWindow::switchMode(bool file)
{
    if(file) {
        defaultLabel->hide();
        return true;
    }
    defaultLabel->show();
    return false;
}

void MainWindow::setupUi(QMainWindow *window) {
    MWBase::setupUi(window);

    retranslateUi(window);
}

void MainWindow::retranslateUi(QMainWindow *) {
}

}
END_PLUG_NAMESPACE
