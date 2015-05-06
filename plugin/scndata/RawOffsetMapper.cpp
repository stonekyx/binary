/*
 * Copyright 2015 KANG Yuxuan
 *
 * RawOffsetMapper.cpp by KANG Yuxuan <stonekyx@gmail.com>
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
