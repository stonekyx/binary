/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_MainWindow.cpp by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/QThread>
#include <QtGui/QRegExpValidator>

#include "CtxJudge.h"
#include "InstData.h"
#include "DisasmTree.h"

#include "ui_MainWindow.h"

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(disasm)
namespace Ui {

MainWindow::MainWindow() :
    MWTreeView("PluginDisasmMainWindow", "Disassemble")
{}

MainWindow::~MainWindow()
{
    hbFileOffRange->deleteLater();
    hbVaddrRange->deleteLater();
}

bool MainWindow::switchMode(bool file)
{
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

class IntentJudge : public CtxJudge {
public:
    IntentJudge(QMainWindow *mw) : CtxJudge(NULL), _mw(mw) {}
    virtual bool operator()(CtxAction *action, const QModelIndex &idx) {
        action->disconnect(SIGNAL(triggered()));
        const InstData &instData = idx.data(Qt::UserRole).value<InstData>();
        switch(instData.addrType) {
            case InstData::AT_NONE:
                return false;
            case InstData::AT_SYMBOL:
                action->setText(QString("Jump to symbol %1")
                        .arg(instData.d.symbol.symName));
                action->setData(idx.data(Qt::UserRole));
                QObject::connect(action, SIGNAL(triggered()),
                        _mw, SLOT(jumpOrOpenSym()));
                break;
            case InstData::AT_VADDR:
                action->setText(QString("Data on address 0x%1")
                        .arg(instData.d.vaddr.addr, 0, 16));
                action->setData(idx.data(Qt::UserRole));
                QObject::connect(action, SIGNAL(triggered()),
                        _mw, SLOT(openScnData()));
                break;
            case InstData::AT_RELOC:
                action->setText(QString("Reloc info of %1")
                        .arg(instData.d.reloc.symName));
                action->setData(idx.data(Qt::UserRole));
                QObject::connect(action, SIGNAL(triggered()),
                        _mw, SLOT(openReloc()));
                break;
            case InstData::AT_SYMBOL_START:
                action->setText(QString("Flow diagram of %1")
                        .arg(instData.d.start.symName));
                action->setData(idx.data(Qt::UserRole));
                QObject::connect(action, SIGNAL(triggered()),
                        _mw, SLOT(openFlowDgm()));
                break;
            default:
                return false;
        }
        return true;
    }
private:
    QMainWindow *_mw;
};

void MainWindow::setupUi(QMainWindow *window) {
    infoTree = new DisasmTree();

    MWTreeView::setupUi(window);
    infoTree->setParent(centralWidget);
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

    CtxAction *actionIntent =
        new CtxAction(new IntentJudge(window), ctxMenu);
    ctxMenu->addAction(actionIntent);
    //no connection!

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

void MainWindow::retranslateUi(QMainWindow *) {
    menuDisasm->setTitle(tr("&Disassemble"));
    actionDisasmStop->setText(tr("&Stop"));
    actionDisasmRefresh->setText(tr("&Refresh"));
    statusLabel->setText(tr("Ready."));
    statusTimerLabel->setText(tr(""));
}

void MainWindow::stopDisasm() {
    emit signalStopDisasm();
}

void MainWindow::refreshDisasm() {
    emit signalRefreshDisasm();
}

void MainWindow::changeStatus() {
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

void MainWindow::setRange(Elf64_Off begin, Elf64_Off end) {
    leFileOffBegin->setText(QString("0x%1").arg(begin, 0, 16));
    leFileOffEnd->setText(QString("0x%1").arg(end, 0, 16));
}

void MainWindow::setVaddrRange(Elf64_Addr begin, Elf64_Addr end) {
    leVaddrBegin->setText(QString("0x%1").arg(begin, 0, 16));
    leVaddrEnd->setText(QString("0x%1").arg(end, 0, 16));
}

void MainWindow::updateRange() {
    if(sender() == leFileOffBegin || sender() == leFileOffEnd) {
        bool ok;
        Elf64_Off begin = leFileOffBegin->text().toULong(&ok, 0);
        if(!ok) { return; }
        Elf64_Off end = leFileOffEnd->text().toULong(&ok, 0);
        if(!ok) { return; }
        emit signalRangeChange(begin, end);
    } else {
        bool ok;
        Elf64_Addr begin = leVaddrBegin->text().toULong(&ok, 0);
        if(!ok) { return; }
        Elf64_Addr end = leVaddrEnd->text().toULong(&ok, 0);
        if(!ok) { return; }
        emit signalVaddrRangeChange(begin, end);
    }
}

void MainWindow::showLoadTime(double sec, double per) {
    statusTimerLabel->setText(QString::number(sec) + "s; " +
            QString::number(per) + "s/inst");
}

}
END_PLUG_NAMESPACE
