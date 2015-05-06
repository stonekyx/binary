/*
 * Copyright 2015 KANG Yuxuan
 *
 * AutoToolTipDelegate.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <QtGui/QHelpEvent>
#include <QtGui/QAbstractItemView>
#include <QtGui/QToolTip>
#include <QtGui/QTextDocument>

#include "AutoToolTipDelegate.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

AutoToolTipDelegate::AutoToolTipDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{}

AutoToolTipDelegate::~AutoToolTipDelegate()
{}

bool AutoToolTipDelegate::helpEvent(QHelpEvent* e,
        QAbstractItemView* view,
        const QStyleOptionViewItem& option,
        const QModelIndex& index)
{
    if(!e || !view)
        return false;

    if(e->type() == QEvent::ToolTip) {
        QRect rect = view->visualRect(index);
        QSize size = sizeHint(option, index);
        if(rect.width() < size.width()) {
            QVariant tooltip = index.data( Qt::DisplayRole );
            if(tooltip.canConvert<QString>()) {
                if(QStyledItemDelegate::helpEvent(e, view, option, index))
                {
                    return true;
                }
                QToolTip::showText(e->globalPos(),
                        QString("<div>%1</div>")
                        .arg(Qt::escape(tooltip.toString())), view);
                return true;
            }
        }
        if(!QStyledItemDelegate::helpEvent(e, view, option, index))
            QToolTip::hideText();
        return true;
    }

    return QStyledItemDelegate::helpEvent(e, view, option, index);
}

END_PLUG_NAMESPACE
