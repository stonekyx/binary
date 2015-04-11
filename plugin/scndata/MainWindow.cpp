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
    updateInfo();
}

MainWindow::~MainWindow()
{ }

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }

    _ui->disconnectTE();

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
        char *buf = new char[shdr.sh_size];
        ssize_t got = file->getScnData(_scnIndex, buf, shdr.sh_size);
        if(got == -1 || (size_t)got != shdr.sh_size) {
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
                    QString("%1").arg((unsigned char)buf[i], 2, 16, QChar('0')).toUpper();
            rawDataStream <<
                    QString("%1").arg(isprint(buf[i])?buf[i]:'.');
        }
        cursor.insertText(hexData, format);
        rawCursor.insertText(rawData, rawFormat);
        addrCursor.insertText(addrData, addrFormat);
        delete[] buf;
    }
    foreach(ScnDataTextEdit *p, _ui->textEdits) {
        QTextCursor cursor(p->textCursor());
        cursor.movePosition(QTextCursor::Start);
        p->setTextCursor(cursor);
    }
    _ui->hexTextEdit->setFocus();
    _ui->connectTE();
}

END_PLUG_NAMESPACE
