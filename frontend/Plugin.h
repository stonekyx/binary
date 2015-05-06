/*
 * Copyright 2015 KANG Yuxuan
 *
 * Plugin.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef FRONTEND_PLUGIN_H
#define FRONTEND_PLUGIN_H

#include <map>
#include <string>

#include "common.h"

#include <QtGui/QAction>
#include <QtGui/QWidget>
#include <QtCore/QObject>

BEGIN_BIN_NAMESPACE(frontend)

class Plugin;
class PluginManager;

END_BIN_NAMESPACE

class binary::frontend::Plugin : public QObject {
    Q_OBJECT
public:
    Plugin() {}
    virtual ~Plugin() = 0;
    virtual const char *getName() = 0;
    virtual QAction *getAction(QWidget *) = 0;
    void *handle;
    PluginManager *manager;
public slots:
    virtual void createView() = 0;
    virtual QObject *createView(std::map<std::string, std::string>) = 0;
protected slots:
    virtual void removeDeadView(QObject *) = 0;
};

inline binary::frontend::Plugin::~Plugin() { }

#endif
