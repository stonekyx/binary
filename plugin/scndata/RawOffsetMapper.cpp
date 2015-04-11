#include <QtGui/QTextCursor>

#include "RawOffsetMapper.h"

BEGIN_PLUG_NAMESPACE(scndata)

RawOffsetMapper::RawOffsetMapper(QTextDocument *td) : _td(td)
{
}

void RawOffsetMapper::toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd)
{
    bool atEnd = false;
    if(curStart == curEnd && _td) {
        QTextCursor cursor(_td);
        cursor.setPosition(curStart);
        if(cursor.atEnd()) {
            atEnd = true;
        }
    }
    offStart = curStart/17*16 + curStart%17;
    offEnd = curEnd/17*16 + curEnd%17;
    if(curStart == curEnd) {
        if(curStart%17 == 16 || atEnd) {
            offStart --;
        } else {
            offEnd ++;
        }
    }
}

void RawOffsetMapper::fromOffset(int &curStart, int &curEnd,
        int offStart, int offEnd)
{
    curStart = offStart/16*17 + offStart%16;
    curEnd = offEnd/16*17 + offEnd%16;
}

END_PLUG_NAMESPACE
