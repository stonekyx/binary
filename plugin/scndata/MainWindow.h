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

#ifndef PLUGIN_SCNDATA_MAINWINDOW_H
#define PLUGIN_SCNDATA_MAINWINDOW_H

#include "frontend/Plugin.h"
#include "backend/File.h"
#include "common.h"
#include "MWBase.h"

#include <QtGui/QMainWindow>

BEGIN_PLUG_NAMESPACE(scndata)

namespace Ui {
    class MainWindow;
}

class MainWindow;

END_PLUG_NAMESPACE

class binary::plugin::scndata::MainWindow :
    public binary::plugin::plugin_framework::MWBase {
    Q_OBJECT
public:
    explicit MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    ~MainWindow();
public:
    virtual void updateInfo(binary::backend::File *);
    using MWBase::updateInfo;
private:
    Ui::MainWindow *_ui;
    size_t _scnIndex;
    Elf64_Off _scnOffset;
    const char *_data;
    size_t _dataSize;
private slots:
    void showDataValues(int, int);
};

#endif
