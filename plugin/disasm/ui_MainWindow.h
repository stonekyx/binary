/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_MainWindow.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_DISASM_UI_MAINWINDOW_H
#define PLUGIN_DISASM_UI_MAINWINDOW_H

#include <elf.h>

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
    void setRange(Elf64_Off begin, Elf64_Off end);
    void setVaddrRange(Elf64_Addr begin, Elf64_Addr end);
    void updateRange();
    void showLoadTime(double sec, double per);
signals:
    void signalStopDisasm();
    void signalRefreshDisasm();
    void signalRangeChange(Elf64_Off, Elf64_Off);
    void signalVaddrRangeChange(Elf64_Addr, Elf64_Addr);
};

#endif
