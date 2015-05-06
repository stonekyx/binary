/*
 * Copyright 2015 KANG Yuxuan
 *
 * MWTreeView.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "ui_MWTreeView.h"

#include "MWTreeView.h"

using namespace std;

USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(plugin_framework)

MWTreeView::MWTreeView(Ui::MWTreeView *ui,
        BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param,
        QWidget *parent) :
    MWBase(ui, plugin, param, parent),
    _ui(ui)
{
}

MWTreeView::~MWTreeView()
{ }

void MWTreeView::updateInfo(File *file)
{
    _ui->switchMode(file);
}

END_PLUG_NAMESPACE
