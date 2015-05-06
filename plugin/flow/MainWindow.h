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

#ifndef PLUGIN_FLOW_MAINWINDOW_H
#define PLUGIN_FLOW_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "common.h"
#include "MWBase.h"
#include "CodeBlock.h"
#include "GVGraph.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QMainWindow>
#include <QtCore/QSet>
#include <QtCore/QList>

BEGIN_PLUG_NAMESPACE(flow)

namespace Ui {
    class MainWindow;
}
class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::flow::MainWindow :
    public binary::plugin::plugin_framework::MWBase
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
    void setAlgo(int);
private:
    bool _noArg;
    size_t _scnIndex;
    Elf64_Off _begin, _end;
    bool _useVaddr;
    Ui::MainWindow *_ui;
    QList<CodeBlock> _blocks;
    QSet<Elf64_Addr> _breaks;
    QList<BIN_NAMESPACE(backend)::File::DisasmInstInfo> _inst;
    Elf64_Addr _vBegin;
    QGraphicsScene *_scene;
    GVGraph::GVAlgoType _algo;
    int _loopCnt;

    static int disasmCallback(
            const BIN_NAMESPACE(backend)::File::DisasmInstInfo &,
            BIN_NAMESPACE(backend)::File::DisasmCBInfo &);
    void generateBlocks();
    std::string outputBlocks();
    int getBlockByStartAddr(Elf64_Addr);
};

#endif
