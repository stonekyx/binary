#ifndef PLUGIN_DISASM_UI_MAINWINDOW_H
#define PLUGIN_DISASM_UI_MAINWINDOW_H

#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QStatusBar>
#include <QtCore/QThread>
#include <QtGui/QRegExpValidator>

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
    QHBoxLayout *hbRangeContainer;
    QLineEdit *leBegin;
    QLineEdit *leEnd;
    QStatusBar *statusBar;
    QLabel *statusLabel;
    QRegExpValidator *rangeValValidator;

    MainWindow() : MWTreeView("PluginDisasmMainWindow", "Disassemble") {}

    virtual bool switchMode(bool file) {
        if(MWTreeView::switchMode(file)) {
            leBegin->show();
            leEnd->show();
            statusBar->show();
            return true;
        }
        leBegin->hide();
        leEnd->hide();
        statusBar->hide();
        return false;
    }

    virtual void setupUi(QMainWindow *window) {
        MWTreeView::setupUi(window);
        window->resize(600, 600);

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

        hbRangeContainer = new QHBoxLayout();
        OBJNAME(hbRangeContainer);
        gridLayout->addLayout(hbRangeContainer, 1, 0, 1, 1);

        rangeValValidator = new QRegExpValidator(QRegExp(
                    "(0x[a-fA-F0-9]+)|([0-9]+)"));

        leBegin = new QLineEdit(centralWidget);
        OBJNAME(leBegin);
        leBegin->hide();
        leBegin->setValidator(rangeValValidator);
        hbRangeContainer->addWidget(leBegin);

        leEnd = new QLineEdit(centralWidget);
        OBJNAME(leEnd);
        leEnd->hide();
        leEnd->setValidator(rangeValValidator);
        hbRangeContainer->addWidget(leEnd);

        statusBar = new QStatusBar(window);
        OBJNAME(statusBar);
        window->setStatusBar(statusBar);

        statusLabel = new QLabel(statusBar);
        OBJNAME(statusLabel);
        statusBar->addWidget(statusLabel);

        QObject::connect(actionDisasmStop, SIGNAL(triggered()),
                this, SLOT(stopDisasm()));
        QObject::connect(actionDisasmRefresh, SIGNAL(triggered()),
                this, SLOT(refreshDisasm()));
        QObject::connect(leBegin, SIGNAL(returnPressed()),
                this, SLOT(updateRange()));
        QObject::connect(leEnd, SIGNAL(returnPressed()),
                this, SLOT(updateRange()));

        retranslateUi(window);
    }
    void retranslateUi(QMainWindow *) {
        menuDisasm->setTitle(tr("&Disassemble"));
        actionDisasmStop->setText(tr("&Stop"));
        actionDisasmRefresh->setText(tr("&Refresh"));
        statusLabel->setText(tr("Ready."));
    }
public slots:
    void stopDisasm() {
        emit signalStopDisasm();
    }
    void refreshDisasm() {
        emit signalRefreshDisasm();
    }
    void changeStatus() {
        QThread *worker = dynamic_cast<QThread*>(sender());
        if(!worker) {
            return;
        }
        if(worker->isFinished()) {
            statusLabel->setText(tr("Ready."));
        } else if(worker->isRunning()) {
            statusLabel->setText(tr("Loading..."));
        } else {
            statusLabel->setText(tr("Error!"));
        }
    }
    void setRange(size_t begin, size_t end) {
        _begin = begin;
        _end = end;
        leBegin->setText(QString("0x%1").arg(begin, 0, 16));
        leEnd->setText(QString("0x%1").arg(end, 0, 16));
    }
    void updateRange() {
        bool ok;
        size_t begin = leBegin->text().toULong(&ok, 0);
        if(!ok) { return; }
        size_t end = leEnd->text().toULong(&ok, 0);
        if(!ok) { return; }
        if(begin != _begin || end != _end) {
            _begin = begin;
            _end = end;
            emit signalRangeChange(begin, end);
        }
    }
signals:
    void signalStopDisasm();
    void signalRefreshDisasm();
    void signalRangeChange(size_t, size_t);
private:
    size_t _begin, _end;
};

#undef OBJNAME
#undef TRANS

#endif
