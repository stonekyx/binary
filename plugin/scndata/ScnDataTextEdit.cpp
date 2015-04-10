#include "ScnDataTextEdit.h"

BEGIN_PLUG_NAMESPACE(scndata)

ScnDataTextEdit::ScnDataTextEdit(QWidget *parent) : QTextEdit(parent)
{
    QObject::connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(calcCursorPos()));
    QObject::connect(this, SIGNAL(selectionChanged()),
            this, SLOT(calcCursorPos()));
}

void ScnDataTextEdit::markCursor(int start, int end, bool recover)
{
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
}

void ScnDataTextEdit::unmarkCursor()
{
    QTextCursor bkp(textCursor());
    if(!_mark.isNull()) {
        setTextCursor(_mark);
        setTextBackgroundColor(Qt::white);
    }
    setTextCursor(bkp);
}

void ScnDataTextEdit::focusOutEvent(QFocusEvent *e)
{
    bool oldState = blockSignals(true);
    unmarkCursor();
    blockSignals(oldState);
    QTextEdit::focusOutEvent(e);
}

void ScnDataTextEdit::calcCursorPos()
{
}

END_PLUG_NAMESPACE
