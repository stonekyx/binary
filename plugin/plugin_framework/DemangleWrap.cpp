/*
 * Copyright 2015 KANG Yuxuan
 *
 * DemangleWrap.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "DemangleWrap.h"
#ifdef LIBIBERTY_DEMANGLE
#include <libiberty/demangle.h>
#elif DEFAULT_DEMANGLE
#include <demangle.h>
#endif

#include "test/DemangleWrapTestExtern.h"

extern "C" {
    extern char *strdup(const char *);
    extern size_t strlen(const char *s);
    extern int strcmp(const char *s1, const char *s2);
}

BEGIN_PLUG_NAMESPACE(plugin_framework)

ssize_t endsWith(const char *mangled, const char *needle)
{
    size_t mlen = strlen(mangled);
    size_t nlen = strlen(needle);
    if(mlen <= nlen) {
        return false;
    }
    return !strcmp(mangled+mlen-nlen, needle) ? mlen-nlen : -1;
}

static const char *cutEnds[] = {"@plt"};

char *cplus_demangle(const char *mangled)
{
    char *alloc = strdup(mangled);
    for(size_t i=0; i<sizeof(cutEnds)/sizeof(cutEnds[0]); i++) {
        ssize_t cutpos = endsWith(mangled, cutEnds[i]);
        if(cutpos != -1) {
            alloc[cutpos] = 0;
            break;
        }
    }
    char *res = ::cplus_demangle(alloc, DMGL_AUTO | DMGL_PARAMS);
    if(!res) {
        return alloc;
    }
    free(alloc);
    return res;
}

END_PLUG_NAMESPACE
