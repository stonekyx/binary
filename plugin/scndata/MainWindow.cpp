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
    _scnOffset = 0;
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 1;
    }
    canUpdateHighlight = true;
    foreach(ScnDataTextEdit *textEdit, _ui->textEdits) {
        QObject::connect(textEdit, SIGNAL(cursorPositionChanged()),
                this, SLOT(updateHighlight()));
        QObject::connect(textEdit, SIGNAL(selectionChanged()),
                this, SLOT(updateHighlight()));
    }
    updateInfo();
}

MainWindow::~MainWindow()
{ }

void MainWindow::updateInfo(File *file)
{
    canUpdateHighlight = false;

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
        char *buf = new char[shdr.sh_size];
        ssize_t got = file->getScnData(_scnIndex, buf, shdr.sh_size);
        if(got == -1 || (size_t)got != shdr.sh_size) {
            return;
        }
        Elf64_Off offset = shdr.sh_offset;
        _scnOffset = offset;
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
    canUpdateHighlight = true;
    _ui->hexTextEdit->setFocus();
}

static inline int textEditEnd(const ScnDataTextEdit *edit)
{
    QTextCursor tmp((edit)->document());
    tmp.movePosition(QTextCursor::End);
    return tmp.position();
}

void MainWindow::updateHighlight()
{
    if(!canUpdateHighlight) {
        return;
    }
    if(_scnOffset == 0) {
        return;
    }
    ScnDataTextEdit *widget = dynamic_cast<ScnDataTextEdit*>(sender());
    if(!widget) {
        return;
    }

    canUpdateHighlight = false;

    QTextCursor cursor(widget->textCursor());
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    int hexEndMax = textEditEnd(_ui->hexTextEdit);
    int rawEndMax = textEditEnd(_ui->rawTextEdit);
    int sliderPosBkp = widget->verticalScrollBar()->sliderPosition();
    if(start!=end) {
        widget->unmarkCursor();
    }
    if(widget == _ui->addrTextEdit) {
        Elf64_Off startLine = (_ui->parseAddr(start)-_scnOffset)/16;
        Elf64_Off endLine = (_ui->parseAddr(end)-_scnOffset)/16;
        if(start == end) {
            QTextCursor lineCursor(_ui->addrTextEdit->textCursor());
            lineCursor.movePosition(QTextCursor::StartOfLine);
            lineCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
            _ui->addrTextEdit->markCursor(lineCursor.selectionStart(),
                    lineCursor.selectionEnd(), true);
        }
        {
            int hexStart = startLine*16*3;
            int hexEnd = (endLine+1)*16*3-1;
            if(hexEnd > hexEndMax) {
                hexEnd = hexEndMax;
            }
            _ui->hexTextEdit->markCursor(hexStart, hexEnd);
        }
        {
            int rawStart = startLine*17;
            int rawEnd = (endLine+1)*17-1;
            if(rawEnd > rawEndMax) {
                rawEnd = rawEndMax;
            }
            _ui->rawTextEdit->markCursor(rawStart, rawEnd);
        }
    } else if(widget == _ui->hexTextEdit) {
        int startLine = start/(16*3);
        int endLine = end/(16*3);
        int startLineOff = start%(16*3);
        int endLineOff = end%(16*3);
        int startByteOff = startLineOff/3+(startLineOff%3>=2);
        int endByteOff = endLineOff/3+1-(endLineOff%3==0);
        if(start == end) {
            startByteOff = startLineOff/3;
            endByteOff = endLineOff/3+1;
            _ui->hexTextEdit->markCursor((startLine*16+startByteOff)*3,
                    (endLine*16+endByteOff)*3-1, true);
        }
        {
            QTextCursor addrCursor(_ui->addrTextEdit->document()->findBlockByLineNumber(startLine));
            QTextCursor addrEndCursor(_ui->addrTextEdit->document()->findBlockByLineNumber(endLine));
            addrEndCursor.movePosition(QTextCursor::EndOfLine);
            addrCursor.movePosition(QTextCursor::StartOfLine);
            _ui->addrTextEdit->markCursor(addrCursor.position(),
                    addrEndCursor.position());
        }
        {
            int rawStart = startLine*17+startByteOff;
            int rawEnd = endLine*17+endByteOff;
            _ui->rawTextEdit->markCursor(rawStart, rawEnd);
        }
    } else if(widget == _ui->rawTextEdit) {
        int startLine = start/17;
        int endLine = end/17;
        int startLineOff = start%17;
        int endLineOff = end%17;
        if(start == end) {
            if((start+1)%17==0 || _ui->rawTextEdit->textCursor().atEnd()) {
                _ui->rawTextEdit->markCursor(start-1, start, true);
                startLineOff = (start-1)%17;
            } else {
                _ui->rawTextEdit->markCursor(start, start+1, true);
                endLineOff = (start+1)%17;
            }
        }
        {
            QTextCursor addrCursor(_ui->addrTextEdit->document()->findBlockByLineNumber(startLine));
            QTextCursor addrEndCursor(_ui->addrTextEdit->document()->findBlockByLineNumber(endLine));
            addrEndCursor.movePosition(QTextCursor::EndOfLine);
            addrCursor.movePosition(QTextCursor::StartOfLine);
            _ui->addrTextEdit->markCursor(addrCursor.position(),
                    addrEndCursor.position());
        }
        {
            int hexStart = startLine*16*3+startLineOff*3;
            int hexEnd = endLine*16*3+endLineOff*3-1;
            _ui->hexTextEdit->markCursor(hexStart, hexEnd);
        }
    }
    canUpdateHighlight = true;
    widget->verticalScrollBar()->setSliderPosition(sliderPosBkp);
}

END_PLUG_NAMESPACE
