/*
 * Copyright 2015 KANG Yuxuan
 *
 * MWBase.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_PLUGIN_FRAMEWORK_MWBASE_H
#define PLUGIN_PLUGIN_FRAMEWORK_MWBASE_H

#include <map>
#include <string>

#include "common.h"
#include "frontend/Plugin.h"
#include "backend/File.h"

#include <QMainWindow>

BEGIN_PLUG_NAMESPACE(plugin_framework)

namespace Ui {
    class MWBase;
}

class MWBase;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::MWBase : public QMainWindow {
    Q_OBJECT
public:
    explicit MWBase(Ui::MWBase *ui,
            BIN_NAMESPACE(frontend)::Plugin *plugin,
            std::map<std::string, std::string> = std::map<std::string, std::string>(),
            QWidget *parent = NULL);
    virtual ~MWBase();
protected:
    virtual void keyPressEvent(QKeyEvent *);
public slots:
    virtual void updateInfo(binary::backend::File *);
    virtual void updateInfo();
protected:
    Ui::MWBase *_ui;
    BIN_NAMESPACE(frontend)::Plugin *_plugin;
};

#endif
