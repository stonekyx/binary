/*
 * Copyright 2015 KANG Yuxuan
 *
 * HexOffsetMapper.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "HexOffsetMapper.h"

BEGIN_PLUG_NAMESPACE(scndata)

void HexOffsetMapper::toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd)
{
    offStart = curStart/(16*3)*16 + curStart%(16*3)/3;
    offEnd = curEnd/(16*3)*16 + curEnd%(16*3)/3 + (curEnd%3>0);
    if(curStart != curEnd) {
        offStart += (curStart%3 >= 2);
    } else {
        offEnd += (curEnd%3==0);
    }
}

void HexOffsetMapper::fromOffset(int &curStart, int &curEnd,
        int offStart, int offEnd)
{
    curStart = offStart*3;
    curEnd = offEnd*3;
    if(curStart != curEnd) {
        curEnd --;
    }
}

END_PLUG_NAMESPACE
