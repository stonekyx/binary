/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_DataValue.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "ui_DataValue.h"

BEGIN_PLUG_NAMESPACE(scndata)
namespace Ui {

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

DataValue::DataValue(const QString &labelText) :
    _lblText(labelText)
{
}

void DataValue::setupUi(QWidget *widget)
{
    label = new QLabel(widget);
    OBJNAME(label);

    lineEdit = new QLineEdit(widget);
    OBJNAME(lineEdit);
    lineEdit->setReadOnly(true);

    hboxLayout = new QHBoxLayout(widget);
    hboxLayout->setContentsMargins(0,0,0,0);
    hboxLayout->addWidget(label, 0);
    hboxLayout->addWidget(lineEdit, 0);

    retranslateUi(widget);
}

void DataValue::retranslateUi(QWidget *)
{
    label->setText(_lblText);
}

void DataValue::setValue(const QString &text)
{
    lineEdit->setText(text);
}

}
END_PLUG_NAMESPACE
