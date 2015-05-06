/*
 * Copyright 2015 KANG Yuxuan
 *
 * InstData.cpp by KANG Yuxuan <stonekyx@gmail.com>
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
#include <cstring>

#include "InstData.h"

BEGIN_PLUG_NAMESPACE(disasm)

InstData::InstData()
{
    memset(&d, 0, sizeof(d));
    addrType = AT_NONE;
}

InstData::InstData(const InstData &o)
{
    *this = o;
}

InstData &InstData::operator=(const InstData &o)
{
    addrType = o.addrType;
    d = o.d;
    switch(addrType) {
    case AT_SYMBOL:
        d.symbol.symName = strdup(o.d.symbol.symName); break;
    case AT_RELOC:
        d.reloc.symName = strdup(o.d.reloc.symName); break;
    case AT_SYMBOL_START:
        d.start.symName = strdup(o.d.start.symName); break;
    default:
        break;
    }
    return *this;
}

InstData::~InstData()
{
    switch(addrType) {
    case AT_SYMBOL:
        free(d.symbol.symName); break;
    case AT_RELOC:
        free(d.reloc.symName); break;
    case AT_SYMBOL_START:
        free(d.start.symName); break;
    default:
        break;
    }
}

END_PLUG_NAMESPACE
