/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_DataValue.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_SCNDATA_UI_DATAVALUE_H
#define PLUGIN_SCNDATA_UI_DATAVALUE_H

#include <elf.h>

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

namespace Ui {
    class DataValue;
}

END_PLUG_NAMESPACE

class binary::plugin::scndata::Ui::DataValue : public QObject
{
    Q_OBJECT
public:
    DataValue(const QString &);

    QHBoxLayout *hboxLayout;
    QLabel *label;
    QLineEdit *lineEdit;

    virtual void setupUi(QWidget *);
    void retranslateUi(QWidget *);
    void setValue(const QString &);
private:
    QString _lblText;
};

#endif
