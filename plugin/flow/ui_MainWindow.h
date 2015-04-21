#ifndef PLUGIN_FLOW_UI_MAINWINDOW_H
#define PLUGIN_FLOW_UI_MAINWINDOW_H

#include <QtGui/QGraphicsView>

#include "ui_MWBase.h"

BEGIN_PLUG_NAMESPACE(flow)

namespace Ui {
    class MainWindow;
}

END_PLUG_NAMESPACE

class binary::plugin::flow::Ui::MainWindow :
    public binary::plugin::plugin_framework::Ui::MWBase
{
public:
    QGraphicsView *graphicsView;

    MainWindow();
    ~MainWindow();

    virtual bool switchMode(bool file);

    virtual void setupUi(QMainWindow *window);

    void retranslateUi(QMainWindow *);
};

#endif
