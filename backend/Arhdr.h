/*
 * Copyright 2015 KANG Yuxuan
 *
 * Arhdr.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_ARHDR_H
#define BACKEND_ARHDR_H

#include <cstdlib>

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class Arhdr {
public:
    Arhdr(const void * = NULL, loff_t = 0);
    Arhdr(const Arhdr &);
    Arhdr &operator=(const Arhdr &);
    ~Arhdr();
    bool isValid() { return _valid; }
    const char *name() { return _name; }
    time_t date() { return _date; }
    uid_t uid() { return _uid; }
    gid_t gid() { return _gid; }
    mode_t mode() { return _mode; }
    loff_t size() { return _size; }
    const char *rawName() { return _rawname; }
    loff_t offset() { return _offset; }
private:
    bool _valid;
    char *_name;		/* Name of archive member.  */
    time_t _date;		/* File date.  */
    uid_t _uid;			/* User ID.  */
    gid_t _gid;			/* Group ID.  */
    mode_t _mode;		/* File mode.  */
    loff_t _size;		/* File size.  */
    char *_rawname;		/* Original name of archive member.  */
    loff_t _offset;
};

END_BIN_NAMESPACE

#endif
