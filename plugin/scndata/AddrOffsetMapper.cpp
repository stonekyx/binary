#include <QtGui/QTextCursor>
#include <QtGui/QTextBlock>

#include "AddrOffsetMapper.h"

BEGIN_PLUG_NAMESPACE(scndata)

AddrOffsetMapper::AddrOffsetMapper(QTextDocument *td) :
    _td(td)
{}

void AddrOffsetMapper::toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd)
{
    QTextCursor cursor(_td);
    cursor.setPosition(curStart);
    offStart = cursor.blockNumber() * 16;
    cursor.setPosition(curEnd);
    offEnd = (cursor.blockNumber()+1) * 16;
}

void AddrOffsetMapper::fromOffset(int &curStart, int &curEnd,
        int offStart, int offEnd)
{
    int startLine = offStart/16;
    int endLine = offEnd/16;
    QTextCursor addrCursor(_td->findBlockByLineNumber(startLine));
    QTextCursor addrEndCursor(_td->findBlockByLineNumber(endLine));
    addrEndCursor.movePosition(QTextCursor::EndOfLine);
    addrCursor.movePosition(QTextCursor::StartOfLine);
    curStart = addrCursor.position();
    curEnd = addrEndCursor.position();
}

END_PLUG_NAMESPACE
