/*
 * Copyright 2015 KANG Yuxuan
 *
 * AutoToolTipDelegate.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_FRAMEWORK_AUTO_TOOLTIP_DELEGATE_H
#define PLUGIN_FRAMEWORK_AUTO_TOOLTIP_DELEGATE_H

#include <QtCore/QObject>
#include <QtGui/QStyledItemDelegate>

#include "common.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

class AutoToolTipDelegate;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::AutoToolTipDelegate :
    public QStyledItemDelegate
{
    Q_OBJECT
public:
    AutoToolTipDelegate(QObject*);
    ~AutoToolTipDelegate();

public slots:
    bool helpEvent(QHelpEvent* e, QAbstractItemView* view,
        const QStyleOptionViewItem& option, const QModelIndex& index);
};

#endif
