/*
 * Copyright 2015 KANG Yuxuan
 *
 * Arsym.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <libelf.h>
#include <cstring>

#include "Arsym.h"

BEGIN_BIN_NAMESPACE(backend)

Arsym::Arsym(const void *srcVoid) :
    _valid(false), _name(NULL), _off(0), _hash(0)
{
    Elf_Arsym *src = (Elf_Arsym*)srcVoid;
    if(!src) {
        return;
    }
    _valid = true;
    _name = src->as_name?strdup(src->as_name):NULL;
    _off = src->as_off;
    _hash = src->as_hash;
}

Arsym::Arsym(const Arsym &o)
{
    *this = o;
}

Arsym &Arsym::operator=(const Arsym &o)
{
    _valid = o._valid;
    _name = o._name?strdup(o._name):NULL;
    _off = o._off;
    _hash = o._hash;
    return *this;
}

Arsym::~Arsym()
{
    if(_name) {
        free(_name);
    }
}

END_BIN_NAMESPACE
