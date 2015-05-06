/*
 * Copyright 2015 KANG Yuxuan
 *
 * Arsym.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_ARSYM_H
#define BACKEND_ARSYM_H

#include <cstdlib>

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Arsym {
public:
    Arsym(const void * = NULL);
    ~Arsym();
    Arsym(const Arsym &);
    Arsym &operator=(const Arsym &);
    bool isValid() { return _valid; }
    const char *name() { return _name; }
    size_t off() { return _off; }
    unsigned long int hash() { return _hash; }
private:
    bool _valid;
    char *_name;
    size_t _off;
    unsigned long int _hash;
};

END_BIN_NAMESPACE

#endif
