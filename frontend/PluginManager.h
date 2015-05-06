/*
 * Copyright 2015 KANG Yuxuan
 *
 * PluginManager.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef FRONTEND_PLUGIN_MANAGER_H
#define FRONTEND_PLUGIN_MANAGER_H

#include <vector>

#include <QtGui/QAction>

#include "common.h"
#include "backend/Backend.h"

BEGIN_BIN_NAMESPACE(frontend)

class Plugin;
class PluginManager;

END_BIN_NAMESPACE

class binary::frontend::PluginManager : public QObject {
    Q_OBJECT
public:
    PluginManager(BIN_NAMESPACE(backend)::Backend *);
    int loadPlugin(const char *);
    QAction *getAction(int, QWidget *);
    void *findSymbol(int, const char *);
    Plugin *getPlugin(const char *);
    Plugin *getPlugin(int);
    BIN_NAMESPACE(backend)::Backend *getBackend();
    void arrangeDelete();
    void registerLibDep(const QObject *, void *);
protected slots:
    void removeLibDep();
private:
    std::vector<Plugin*> _plugins;
    BIN_NAMESPACE(backend)::Backend *_backend;
    std::map<const QObject *, void *> _libDep;
    std::map<void *, int> _depCnt;
    bool _arrangedDelete;
    ~PluginManager();
};

#endif
