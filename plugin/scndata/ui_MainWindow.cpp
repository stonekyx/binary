/*
 * Copyright 2015 KANG Yuxuan
 *
 * ui_MainWindow.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "AddrOffsetMapper.h"
#include "HexOffsetMapper.h"
#include "RawOffsetMapper.h"

#include "DataValueConvertTypes.h"

#include "ui_MainWindow.h"

BEGIN_PLUG_NAMESPACE(scndata)
namespace Ui {

#define OBJNAME(widget) \
        do{ widget->setObjectName(QString::fromUtf8(#widget)); }while(0)

#define TRANS(raw) \
        QApplication::translate(_context, raw, 0, QApplication::UnicodeUTF8)

MainWindow::~MainWindow()
{
    delete dataAreaLayout;
}

bool MainWindow::switchMode(bool file)
{
    if(!file) {
        foreach(ScnDataTextEdit *p, textEdits) {
            p->hide();
        }
        foreach(DataValue *p, dataValues) {
            p->hide();
        }
        defaultLabel->show();
        return false;
    }
    defaultLabel->hide();
    foreach(ScnDataTextEdit *p, textEdits) {
        p->show();
        p->clear();
    }
    foreach(DataValue *p, dataValues) {
        p->show();
        p->clear();
    }
    return true;
}

void MainWindow::setupUi(QMainWindow *window)
{
    MWBase::setupUi(window);

    window->resize(650, 500);
    window->setMinimumWidth(650);
    window->setMinimumHeight(500);

    hexTextEdit = new ScnDataTextEdit(new HexOffsetMapper(), centralWidget);
    OBJNAME(hexTextEdit);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(5);
    sizePolicy.setVerticalStretch(5);
    sizePolicy.setHeightForWidth(hexTextEdit->sizePolicy().hasHeightForWidth());
    hexTextEdit->setSizePolicy(sizePolicy);
    hexTextEdit->setReadOnly(true);
    hexTextEdit->setTextInteractionFlags(
            hexTextEdit->textInteractionFlags() |
            Qt::TextSelectableByKeyboard);

    rawTextEdit = new ScnDataTextEdit(NULL, centralWidget);
    OBJNAME(rawTextEdit);
    rawTextEdit->setOffsetMapper(new RawOffsetMapper(rawTextEdit->document()));
    sizePolicy.setHorizontalStretch(2);
    sizePolicy.setVerticalStretch(2);
    sizePolicy.setHeightForWidth(rawTextEdit->sizePolicy().hasHeightForWidth());
    rawTextEdit->setSizePolicy(sizePolicy);
    rawTextEdit->setReadOnly(true);
    rawTextEdit->setTextInteractionFlags(
            rawTextEdit->textInteractionFlags() |
            Qt::TextSelectableByKeyboard);

    addrTextEdit = new ScnDataTextEdit(NULL, centralWidget);
    OBJNAME(addrTextEdit);
    addrTextEdit->setOffsetMapper(new AddrOffsetMapper(addrTextEdit->document()));
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(1);
    sizePolicy.setHeightForWidth(addrTextEdit->sizePolicy().hasHeightForWidth());
    addrTextEdit->setSizePolicy(sizePolicy);
    addrTextEdit->setReadOnly(true);
    addrTextEdit->setTextInteractionFlags(
            addrTextEdit->textInteractionFlags() |
            Qt::TextSelectableByKeyboard);
    addrTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gridLayout->addWidget(hexTextEdit, 0, 1, 1, 1);
    gridLayout->addWidget(rawTextEdit, 0, 2, 1, 1);
    gridLayout->addWidget(addrTextEdit, 0, 0, 1, 1);
    gridLayout->setRowStretch(0, 3);

    textEdits.push_back(hexTextEdit);
    textEdits.push_back(rawTextEdit);
    textEdits.push_back(addrTextEdit);

    dataValues.push_back(new DataValue("Signed int8",
                new DataValueConvertTypes<int8_t>, centralWidget));
    dataValues.push_back(new DataValue("Unsigned int8",
                new DataValueConvertTypes<uint8_t>, centralWidget));
    dataValues.push_back(new DataValue("Signed int16",
                new DataValueConvertTypes<int16_t>, centralWidget));
    dataValues.push_back(new DataValue("Unsigned int16",
                new DataValueConvertTypes<uint16_t>, centralWidget));
    dataValues.push_back(new DataValue("Signed int32",
                new DataValueConvertTypes<int32_t>, centralWidget));
    dataValues.push_back(new DataValue("Unsigned int32",
                new DataValueConvertTypes<uint32_t>, centralWidget));
    dataValues.push_back(new DataValue("Signed int64",
                new DataValueConvertTypes<int64_t>, centralWidget));
    dataValues.push_back(new DataValue("Unsigned int64",
                new DataValueConvertTypes<uint64_t>, centralWidget));
    dataValues.push_back(new DataValue("Float",
                new DataValueConvertTypes<float>, centralWidget));
    dataValues.push_back(new DataValue("Double",
                new DataValueConvertTypes<double>, centralWidget));
    dataValues.push_back(new DataValue("Long double",
                new DataValueConvertTypes<long double>, centralWidget));
    dataValues.push_back(new DataValue("String",
                new DataValueConvertTypes<char*>, centralWidget));

    dataAreaLayout = new QGridLayout();
    gridLayout->addLayout(dataAreaLayout, 1, 0, 1, 3);
    dataAreaLayout->setVerticalSpacing(0);
    dataAreaLayout->setColumnStretch(0, 1);
    dataAreaLayout->setColumnStretch(1, 1);
    for(int i=0; i<dataValues.size(); i++) {
        dataAreaLayout->addWidget(dataValues[i], i/2, i%2, 1, 1);
    }

    for(int i=1; i<textEdits.size(); i++) {
        QObject::connect(textEdits[i]->verticalScrollBar(),
                SIGNAL(valueChanged(int)),
                textEdits[i-1]->verticalScrollBar(),
                SLOT(setValue(int)));
        QObject::connect(textEdits[i-1]->verticalScrollBar(),
                SIGNAL(valueChanged(int)),
                textEdits[i]->verticalScrollBar(),
                SLOT(setValue(int)));
    }
    foreach(ScnDataTextEdit *p, textEdits) {
        p->ensureCursorVisible();
        p->listenGroup(textEdits);
    }
    foreach(DataValue *p, dataValues) {
        QObject::connect(this, SIGNAL(signalDataValue(const char *, size_t)),
                p, SLOT(setValue(const char *, size_t)));
    }

    retranslateUi(window);
}

void MainWindow::connectTE()
{
    foreach(ScnDataTextEdit *p, textEdits) {
        p->setBlockOM(false);
    }
}

void MainWindow::disconnectTE()
{
    foreach(ScnDataTextEdit *p, textEdits) {
        p->setBlockOM(true);
    }
}

void MainWindow::setDataValue(const char *data, size_t size)
{
    emit signalDataValue(data, size);
}

}
END_PLUG_NAMESPACE
