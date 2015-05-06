/*
 * Copyright 2015 KANG Yuxuan
 *
 * Colors.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <cstdlib>
#include <ctime>

#include "Colors.h"

BEGIN_PLUG_NAMESPACE(layout)

static int randRange(int min, int max)
{
    return rand()%(max-min+1)+min;
}

Colors::Colors(int level) : _level(level)
{
    static bool inited = false;
    if(!inited) {
        srand(time(NULL));
        inited = true;
    }
    _s = randRange(80,170);
    _l = randRange(80,170);
}

QColor Colors::next()
{
    int h = randRange(0, _level);
    h = 150.0/_level * h;
    if(h<0) h=0;
    if(h>149) h=149;
    h += randRange(0, 210);
    return QColor::fromHsl(h, _s, _l);
}

END_PLUG_NAMESPACE
