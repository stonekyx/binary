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

    MainWindow() : MWTreeView("PluginDisasmMainWindow", "Disassemble") {}
    ~MainWindow() {
        hbFileOffRange->deleteLater();
        hbVaddrRange->deleteLater();
    }

    virtual bool switchMode(bool file) {
        if(MWTreeView::switchMode(file)) {
            leFileOffBegin->show();
            leFileOffEnd->show();
            leVaddrBegin->show();
            leVaddrEnd->show();
            statusBar->show();
            return true;
        }
        leFileOffBegin->hide();
        leFileOffEnd->hide();
        leVaddrBegin->hide();
        leVaddrEnd->hide();
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

        hbFileOffRange = new QHBoxLayout();
        OBJNAME(hbFileOffRange);
        gridLayout->addLayout(hbFileOffRange, 1, 0, 1, 1);

        rangeValValidator = new QRegExpValidator(QRegExp(
                    "(0x[a-fA-F0-9]+)|([0-9]+)"));

        leFileOffBegin = new QLineEdit(centralWidget);
        OBJNAME(leFileOffBegin);
        leFileOffBegin->hide();
        leFileOffBegin->setValidator(rangeValValidator);
        hbFileOffRange->addWidget(leFileOffBegin);

        leFileOffEnd = new QLineEdit(centralWidget);
        OBJNAME(leFileOffEnd);
        leFileOffEnd->hide();
        leFileOffEnd->setValidator(rangeValValidator);
        hbFileOffRange->addWidget(leFileOffEnd);

        hbVaddrRange = new QHBoxLayout();
        OBJNAME(hbVaddrRange);
        gridLayout->addLayout(hbVaddrRange, 2, 0, 1, 1);

        leVaddrBegin = new QLineEdit(centralWidget);
        OBJNAME(leVaddrBegin);
        leVaddrBegin->hide();
        leVaddrBegin->setValidator(rangeValValidator);
        hbVaddrRange->addWidget(leVaddrBegin);

        leVaddrEnd = new QLineEdit(centralWidget);
        OBJNAME(leVaddrEnd);
        leVaddrEnd->hide();
        leVaddrEnd->setValidator(rangeValValidator);
        hbVaddrRange->addWidget(leVaddrEnd);

        statusBar = new QStatusBar(window);
        OBJNAME(statusBar);
        window->setStatusBar(statusBar);

        statusLabel = new QLabel(statusBar);
        OBJNAME(statusLabel);
        statusBar->addWidget(statusLabel);

        statusTimerLabel = new QLabel(statusBar);
        OBJNAME(statusTimerLabel);
        statusBar->addPermanentWidget(statusTimerLabel);

        QObject::connect(actionDisasmStop, SIGNAL(triggered()),
                this, SLOT(stopDisasm()));
        QObject::connect(actionDisasmRefresh, SIGNAL(triggered()),
                this, SLOT(refreshDisasm()));
        QObject::connect(leFileOffBegin, SIGNAL(returnPressed()),
                this, SLOT(updateRange()));
        QObject::connect(leFileOffEnd, SIGNAL(returnPressed()),
                this, SLOT(updateRange()));
        QObject::connect(leVaddrBegin, SIGNAL(returnPressed()),
                this, SLOT(updateRange()));
        QObject::connect(leVaddrEnd, SIGNAL(returnPressed()),
                this, SLOT(updateRange()));

        retranslateUi(window);
    }
    void retranslateUi(QMainWindow *) {
        menuDisasm->setTitle(tr("&Disassemble"));
        actionDisasmStop->setText(tr("&Stop"));
        actionDisasmRefresh->setText(tr("&Refresh"));
        statusLabel->setText(tr("Ready."));
        statusTimerLabel->setText(tr(""));
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
        leFileOffBegin->setText(QString("0x%1").arg(begin, 0, 16));
        leFileOffEnd->setText(QString("0x%1").arg(end, 0, 16));
    }
    void setVaddrRange(size_t begin, size_t end) {
        leVaddrBegin->setText(QString("0x%1").arg(begin, 0, 16));
        leVaddrEnd->setText(QString("0x%1").arg(end, 0, 16));
    }
    void updateRange() {
        if(sender() == leFileOffBegin || sender() == leFileOffEnd) {
            bool ok;
            size_t begin = leFileOffBegin->text().toULong(&ok, 0);
            if(!ok) { return; }
            size_t end = leFileOffEnd->text().toULong(&ok, 0);
            if(!ok) { return; }
            emit signalRangeChange(begin, end);
        } else {
            bool ok;
            size_t begin = leVaddrBegin->text().toULong(&ok, 0);
            if(!ok) { return; }
            size_t end = leVaddrEnd->text().toULong(&ok, 0);
            if(!ok) { return; }
            emit signalVaddrRangeChange(begin, end);
        }
    }
    void showLoadTime(double sec, double per) {
        statusTimerLabel->setText(QString::number(sec) + "s; " +
                QString::number(per) + "s/inst");
    }
signals:
    void signalStopDisasm();
    void signalRefreshDisasm();
    void signalRangeChange(size_t, size_t);
    void signalVaddrRangeChange(size_t, size_t);
};

#undef OBJNAME
#undef TRANS

#endif
