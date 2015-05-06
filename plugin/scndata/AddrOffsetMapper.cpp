/*
 * Copyright 2015 KANG Yuxuan
 *
 * AddrOffsetMapper.cpp by KANG Yuxuan <stonekyx@gmail.com>
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
    int endLine = (offEnd-1)/16;
    QTextCursor addrCursor(_td->findBlockByLineNumber(startLine));
    QTextCursor addrEndCursor(_td->findBlockByLineNumber(endLine));
    addrEndCursor.movePosition(QTextCursor::EndOfLine);
    addrCursor.movePosition(QTextCursor::StartOfLine);
    curStart = addrCursor.position();
    curEnd = addrEndCursor.position();
}

END_PLUG_NAMESPACE
