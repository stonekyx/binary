/*
 * Copyright 2015 KANG Yuxuan
 *
 * MWBase.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <iostream>
#include <cstdio>
#include <QKeyEvent>
#include <QtGui/QHeaderView>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"

#include "ui_MWBase.h"

#include "MWBase.h"

using namespace std;

USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(plugin_framework)

MWBase::MWBase(Ui::MWBase *ui,
        BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string>,
        QWidget *parent) :
    QMainWindow(parent),
    _ui(ui),
    _plugin(plugin)
{
    _ui->setupUi(this);
    QObject::connect(_plugin->manager->getBackend(),
            SIGNAL(fileChanged(binary::backend::File *)),
            this, SLOT(updateInfo(binary::backend::File *)));
    setAttribute(Qt::WA_DeleteOnClose);
    show();
}

MWBase::~MWBase()
{
    delete _ui;
}

void MWBase::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_W:
        if(event->modifiers() & Qt::ControlModifier)
            close();
        break;
    default:
        break;
    }
}

void MWBase::updateInfo(File *)
{
}

void MWBase::updateInfo()
{
    updateInfo(_plugin->manager->getBackend()->getFile());
}

END_PLUG_NAMESPACE
