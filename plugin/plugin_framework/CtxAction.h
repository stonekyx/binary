/*
 * Copyright 2015 KANG Yuxuan
 *
 * CtxAction.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_FRAMEWORK_CTX_ACTION_H
#define PLUGIN_FRAMEWORK_CTX_ACTION_H

#include <QtGui/QAction>

#include "common.h"

class QModelIndex;

BEGIN_PLUG_NAMESPACE(plugin_framework)

class CtxJudge;
class CtxAction;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::CtxAction :
    public QAction
{
public:
    CtxAction(CtxJudge * = NULL, QWidget * = NULL);
    ~CtxAction();
    bool judgeVisible(const QModelIndex &);
private:
    CtxJudge *_judge;
};

#endif
