#ifndef PLUGIN_DISASM_UI_MAINWINDOW_H
#define PLUGIN_DISASM_UI_MAINWINDOW_H

#include "ui_MWTreeView.h"

BEGIN_PLUG_NAMESPACE(disasm)

namespace Ui {
    class MainWindow;
}

END_PLUG_NAMESPACE

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

class binary::plugin::disasm::Ui::MainWindow :
    public binary::plugin::plugin_framework::Ui::MWTreeView
{
    Q_OBJECT
public:
    QMenu *menuDisasm;
    QAction *actionDisasmStop;
    QAction *actionDisasmRefresh;

    MainWindow() : MWTreeView("PluginDisasmMainWindow", "Disassemble") {}

    virtual void setupUi(QMainWindow *window) {
        MWTreeView::setupUi(window);

        menuDisasm = new QMenu(menuBar);
        OBJNAME(menuDisasm);
        menuBar->addAction(menuDisasm->menuAction());

        actionDisasmStop = new QAction(window);
        OBJNAME(actionDisasmStop);
        actionDisasmStop->setShortcut(QKeySequence(Qt::Key_Escape));

        actionDisasmRefresh = new QAction(window);
        OBJNAME(actionDisasmRefresh);
        actionDisasmRefresh->setShortcut(QKeySequence::Refresh);

        menuDisasm->addAction(actionDisasmStop);
        menuDisasm->addAction(actionDisasmRefresh);

        QObject::connect(actionDisasmStop, SIGNAL(triggered()),
                this, SLOT(stopDisasm()));
        QObject::connect(actionDisasmRefresh, SIGNAL(triggered()),
                this, SLOT(refreshDisasm()));

        retranslateUi(window);
    }
    void retranslateUi(QMainWindow *) {
        menuDisasm->setTitle(tr("&Disassemble"));
        actionDisasmStop->setText(tr("&Stop"));
        actionDisasmRefresh->setText(tr("&Refresh"));
    }
public slots:
    void stopDisasm() {
        emit signalStopDisasm();
    }
    void refreshDisasm() {
        emit signalRefreshDisasm();
    }
signals:
    void signalStopDisasm();
    void signalRefreshDisasm();
};

#undef OBJNAME
#undef TRANS

#endif
