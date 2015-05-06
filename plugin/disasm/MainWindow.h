/*
 * Copyright 2015 KANG Yuxuan
 *
 * MainWindow.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_DISASM_MAINWINDOW_H
#define PLUGIN_DISASM_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "common.h"
#include "MWTreeView.h"
#include "InfoModel.h"

#include <QtGui/QMainWindow>

#include "LoadWorker.h"

BEGIN_PLUG_NAMESPACE(disasm)

namespace Ui {
    class MainWindow;
}
class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::disasm::MainWindow :
    public binary::plugin::plugin_framework::MWTreeView
{
    Q_OBJECT
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    ~MainWindow();
public:
    virtual void updateInfo(binary::backend::File *);
    using MWBase::updateInfo;
public slots:
    void spanFirstColumn(QModelIndex);
    void resetWorker();
    void setRange(Elf64_Off, Elf64_Off);
    void setVaddrRange(Elf64_Addr, Elf64_Addr);
    void openScnData();
    void jumpOrOpenSym();
    void openReloc();
    void openFlowDgm();
private:
    PLUG_NAMESPACE(plugin_framework)::InfoModel *_infoModel;
    size_t _scnIndex;
    LoadWorker *_loadWorker;
    Ui::MainWindow *_ui;
    bool _useRange, _useVRange;
    Elf64_Off _begin, _end;
};

#endif
