#ifndef PLUGIN_DISASM_UI_MAINWINDOW_H
#define PLUGIN_DISASM_UI_MAINWINDOW_H

#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QStatusBar>

#include "CtxAction.h"
#include "ui_MWTreeView.h"

BEGIN_PLUG_NAMESPACE(disasm)

namespace Ui {
    class MainWindow;
}

END_PLUG_NAMESPACE

class binary::plugin::disasm::Ui::MainWindow :
    public binary::plugin::plugin_framework::Ui::MWTreeView
{
    Q_OBJECT
public:
    QMenu *menuDisasm;
    QAction *actionDisasmStop;
    QAction *actionDisasmRefresh;
    QHBoxLayout *hbFileOffRange;
    QLineEdit *leFileOffBegin;
    QLineEdit *leFileOffEnd;
    QHBoxLayout *hbVaddrRange;
    QLineEdit *leVaddrBegin;
    QLineEdit *leVaddrEnd;
    QStatusBar *statusBar;
    QLabel *statusLabel;
    QLabel *statusTimerLabel;
    QRegExpValidator *rangeValValidator;

    MainWindow();
    ~MainWindow();

    virtual bool switchMode(bool file);

    virtual void setupUi(QMainWindow *window);

    void retranslateUi(QMainWindow *);
public slots:
    void stopDisasm();
    void refreshDisasm();
    void changeStatus();
    void setRange(size_t begin, size_t end);
    void setVaddrRange(size_t begin, size_t end);
    void updateRange();
    void showLoadTime(double sec, double per);
signals:
    void signalStopDisasm();
    void signalRefreshDisasm();
    void signalRangeChange(size_t, size_t);
    void signalVaddrRangeChange(size_t, size_t);
};

#endif
