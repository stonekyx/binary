#include "GraphicsView.h"

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
        graphicsView->show();
        statusBar->show();
        return true;
    }
    graphicsView->hide();
    statusBar->hide();
    defaultLabel->show();
    return false;
}

void MainWindow::setupUi(QMainWindow *window) {
    MWBase::setupUi(window);

    graphicsView = new GraphicsView(centralWidget);
    OBJNAME(graphicsView);
    graphicsView->resetTransform();
    gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

    statusBar = new QStatusBar(window);
    OBJNAME(statusBar);
    window->setStatusBar(statusBar);

    lblMousePos = new QLabel(statusBar);
    OBJNAME(lblMousePos);
    statusBar->addPermanentWidget(lblMousePos);

    QObject::connect(graphicsView, SIGNAL(mouseScenePos(const QString&)),
            lblMousePos, SLOT(setText(const QString&)));

    retranslateUi(window);
}

void MainWindow::retranslateUi(QMainWindow *) {
}

}
END_PLUG_NAMESPACE
