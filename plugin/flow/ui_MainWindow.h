#ifndef PLUGIN_FLOW_UI_MAINWINDOW_H
#define PLUGIN_FLOW_UI_MAINWINDOW_H

#include <QtGui/QStatusBar>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QButtonGroup>

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
    Q_OBJECT
public:
    GraphicsView *graphicsView;
    QStatusBar *statusBar;
    QLabel *lblMousePos;
    QLabel *lblLoopCnt;
    QButtonGroup *btnGrpAlgo;
    QRadioButton *rbtnAlgoFdp, *rbtnAlgoDot;
    QSlider *scaleSlider;
    QPushButton *btnResetSlider;

    MainWindow();
    ~MainWindow();

    virtual bool switchMode(bool file);

    virtual void setupUi(QMainWindow *window);

    void retranslateUi(QMainWindow *);
public slots:
    void resetSlider();
    void scaleView(int);
    void setLoopCnt(int);
private:
    int _sliderMin, _sliderMax;
};

#endif
