/*
 * Copyright 2015 KANG Yuxuan
 *
 * DataValue.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "DataValueConvert.h"
#include "ui_DataValue.h"

#include "DataValue.h"

BEGIN_PLUG_NAMESPACE(scndata)

class DataValue::Private {
public:
    Ui::DataValue *_ui;
    DataValueConvert *_dvc;
};

DataValue::DataValue(const QString &lblText, DataValueConvert *dvc,
        QWidget *parent) :
    QWidget(parent), _priv(new Private())
{
    _priv->_ui = new Ui::DataValue(lblText);
    _priv->_dvc = dvc;
    _priv->_ui->setupUi(this);
}

DataValue::~DataValue()
{
    delete _priv->_ui;
    delete _priv->_dvc;
    delete _priv;
}

void DataValue::clear()
{
    _priv->_ui->setValue(QString());
}

void DataValue::setValue(const char *data, size_t size)
{
    if(!_priv->_dvc) {
        return;
    }
    _priv->_ui->setValue(_priv->_dvc->toString(data, size));
}

END_PLUG_NAMESPACE
