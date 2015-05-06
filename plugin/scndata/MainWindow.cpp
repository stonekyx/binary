/*
 * Copyright 2015 KANG Yuxuan
 *
 * MainWindow.cpp by KANG Yuxuan <stonekyx@gmail.com>
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
#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtGui/QTextBlock>

#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;
USE_BIN_NAMESPACE(frontend);
USE_BIN_NAMESPACE(backend);

BEGIN_PLUG_NAMESPACE(scndata)

MainWindow::MainWindow(Plugin *plugin, map<string, string> param,
        QWidget *parent) :
    MWBase(new Ui::MainWindow(), plugin, param, parent),
    _ui(dynamic_cast<Ui::MainWindow*>(MWBase::_ui))
{
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 1;
    }
    if(param.find("scnOffset") != param.end()) {
        _scnOffset = QString(param["scnOffset"].c_str()).toULong();
    } else {
        _scnOffset = 0;
    }
    _data = NULL; _dataSize = 0;
    foreach(ScnDataTextEdit *p, _ui->textEdits) {
        QObject::connect(p, SIGNAL(offsetMapped(int, int)),
                this, SLOT(showDataValues(int, int)));
    }
    updateInfo();
}

MainWindow::~MainWindow()
{ }

void MainWindow::updateInfo(File *file)
{
    _ui->disconnectTE();

    if(!_ui->switchMode(file)) {
        return;
    }

    QTextCursor cursor(_ui->hexTextEdit->textCursor());
    cursor.movePosition(QTextCursor::Start);

    QTextCharFormat format(cursor.charFormat());
    format.setFontFamily("Courier");

    QTextCursor rawCursor(_ui->rawTextEdit->textCursor());
    rawCursor.movePosition(QTextCursor::Start);

    QTextCharFormat rawFormat(rawCursor.charFormat());
    rawFormat.setFontFamily("Courier");

    QTextCursor addrCursor(_ui->addrTextEdit->textCursor());
    addrCursor.movePosition(QTextCursor::Start);

    QTextCharFormat addrFormat(addrCursor.charFormat());
    addrFormat.setFontFamily("Courier");

    {
        Elf64_Shdr shdr;
        if(!file->getShdr(_scnIndex, &shdr)) {
            return;
        }
        _data = file->getRawData(shdr.sh_offset);
        _dataSize = shdr.sh_size;
        if(!_data) {
            return;
        }
        Elf64_Off offset = shdr.sh_offset;
        QString hexData;
        QString rawData;
        QString addrData;
        QTextStream hexDataStream(&hexData);
        QTextStream rawDataStream(&rawData);
        QTextStream addrDataStream(&addrData);
        double lastPercent = 0;
        for(size_t i=0; i<shdr.sh_size; i++) {
            if(i && i%16 == 0) {
                hexDataStream << "\n";
                rawDataStream << "\n";
                addrDataStream << "\n";
            }
            if(i%16 == 0) {
                addrDataStream << QString("0x%1").arg(offset, 0, 16) << ":";
                offset += 16;
                if(shdr.sh_size>100000 &&
                        (double)i/shdr.sh_size - lastPercent >= 0.1) {
                    lastPercent = (double)i/shdr.sh_size;
                    cout<<"Loading..."<<(int)(lastPercent*100)<<"%"<<endl;
                }
            }
            if(i%16) { // not line beginning
                hexDataStream << " ";
            }
            hexDataStream <<
                    QString("%1").arg((unsigned char)_data[i], 2, 16, QChar('0')).toUpper();
            rawDataStream <<
                    QString("%1").arg(isprint(_data[i])?_data[i]:'.');
        }
        cursor.insertText(hexData, format);
        rawCursor.insertText(rawData, rawFormat);
        addrCursor.insertText(addrData, addrFormat);
    }
    foreach(ScnDataTextEdit *p, _ui->textEdits) {
        QTextCursor cursor(p->textCursor());
        cursor.movePosition(QTextCursor::Start);
        p->setTextCursor(cursor);
    }
    _ui->hexTextEdit->setFocus();
    _ui->connectTE();
    _ui->hexTextEdit->changeCursorPosInit(_scnOffset, _scnOffset);
}

void MainWindow::showDataValues(int offset, int)
{
    if(!_data || (size_t)offset>_dataSize) {
        return;
    }
    _ui->setDataValue(_data+offset, _dataSize - offset);
}

END_PLUG_NAMESPACE
