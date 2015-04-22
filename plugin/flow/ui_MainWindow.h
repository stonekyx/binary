#ifndef PLUGIN_FLOW_UI_MAINWINDOW_H
#define PLUGIN_FLOW_UI_MAINWINDOW_H

#include <QtGui/QStatusBar>
#include <QtGui/QLabel>

#include "ui_MWBase.h"

BEGIN_PLUG_NAMESPACE(flow)

namespace Ui {
    class MainWindow;
    class GraphicsView;
}

END_PLUG_NAMESPACE

class binary::plugin::flow::Ui::MainWindow :
    public binary::plugin::plugin_framework::Ui::MWBase
{
public:
    GraphicsView *graphicsView;
    QStatusBar *statusBar;
    QLabel *lblMousePos;

    MainWindow();
    ~MainWindow();

    virtual bool switchMode(bool file);

    virtual void setupUi(QMainWindow *window);

    void retranslateUi(QMainWindow *);
};

#endif
