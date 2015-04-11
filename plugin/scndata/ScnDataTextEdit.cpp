#include <QtGui/QTextCursor>
#include <QtGui/QScrollBar>

#include "OffsetMapper.h"
#include "ScnDataTextEdit.h"

BEGIN_PLUG_NAMESPACE(scndata)

ScnDataTextEdit::ScnDataTextEdit(OffsetMapper *om, QWidget *parent) :
    QTextEdit(parent), _om(om)
{
    QObject::connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(calcCursorPos()));
    QObject::connect(this, SIGNAL(selectionChanged()),
            this, SLOT(calcCursorPos()));
}

ScnDataTextEdit::~ScnDataTextEdit()
{
    if(_om) {
        delete _om;
    }
}

void ScnDataTextEdit::markCursor(int start, int end, bool recover)
{
    setBlockOM(true);
    QTextCursor bkp(textCursor());
    if(!_mark.isNull()) {
        setTextCursor(_mark);
        setTextBackgroundColor(Qt::white);
    }
    _mark = bkp;
    _mark.setPosition(start);
    _mark.setPosition(end, QTextCursor::KeepAnchor);
    setTextCursor(_mark);
    setTextBackgroundColor(Qt::lightGray);
    if(recover) {
        setTextCursor(bkp);
    }
    setBlockOM(false);
}

void ScnDataTextEdit::unmarkCursor()
{
    setBlockOM(true);
    QTextCursor bkp(textCursor());
    if(!_mark.isNull()) {
        setTextCursor(_mark);
        setTextBackgroundColor(Qt::white);
    }
    setTextCursor(bkp);
    setBlockOM(false);
}

void ScnDataTextEdit::setOffsetMapper(OffsetMapper *om)
{
    _om = om;
}

void ScnDataTextEdit::listenGroup(const QList<ScnDataTextEdit*> &group)
{
    foreach(const ScnDataTextEdit *m, group) {
        if(m == this) {
            continue;
        }
        QObject::connect(m, SIGNAL(offsetMapped(int, int)),
                this, SLOT(changeCursorPos(int, int)));
    }
}

void ScnDataTextEdit::setBlockOM(bool block)
{
    _blockOM = block;
}

void ScnDataTextEdit::calcCursorPos()
{
    if(_blockOM || !_om) {
        return;
    }
    int sliderPosBkp = verticalScrollBar()->sliderPosition();
    int cursor = textCursor().selectionStart();
    int cursorE = textCursor().selectionEnd();
    int offset, offsetE;
    _om->toOffset(offset, offsetE, cursor, cursorE);
    emit offsetMapped(offset, offsetE);
    if(cursor == cursorE) {
        changeCursorPos(offset, offsetE, true);
    } else {
        unmarkCursor();
    }
    verticalScrollBar()->setSliderPosition(sliderPosBkp);
}

int ScnDataTextEdit::lastPos()
{
    QTextCursor cursor(textCursor());
    cursor.movePosition(QTextCursor::End);
    return cursor.position();
}

void ScnDataTextEdit::changeCursorPos(int offset, int offsetE, bool rec)
{
    if(_blockOM || !_om) {
        return;
    }
    int cursor, cursorE;
    _om->fromOffset(cursor, cursorE, offset, offsetE);
    int max = lastPos();
    if(cursor>max) { cursor = max; }
    if(cursorE>max) { cursorE = max; }
    markCursor(cursor, cursorE, rec);
}

END_PLUG_NAMESPACE
