/*
 * Copyright 2015 KANG Yuxuan
 *
 * Defines.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <cstring>

#include "Defines.h"

#include "defines/all.h"

using namespace std;
BEGIN_PLUG_NAMESPACE(plugin_framework)

template<typename T, typename U, typename V>
V *anyTextSlow(T val,
        ExpandDefine<T, U> &obj, V *DefineInfo<U>::*memb, V *failsafe)
{
    if(obj.defineMap.find(val) != obj.defineMap.end()) {
        return obj.defineMap[val].*memb;
    }
    const DefineInfo<U> &info = obj.queryRangeSlow(val);
    if(info.valid) {
        return info.*memb;
    }
    return failsafe;
}

vector<const char *> Defines::macroText_SHF(Elf64_Xword val)
{
    vector<const char *> res;
    for(map<Elf64_Xword, DefineInfo<> >::iterator it = defines_SHF.defineMap.begin();
            it != defines_SHF.defineMap.end(); it++)
    {
        if(val & it->first) {
            res.push_back(it->second.name);
        }
    }
    return res;
}

vector<const char *> Defines::commentText_SHF(Elf64_Xword val)
{
    vector<const char *> res;
    for(map<Elf64_Xword, DefineInfo<> >::iterator it = defines_SHF.defineMap.begin();
            it != defines_SHF.defineMap.end(); it++)
    {
        if(val & it->first) {
            res.push_back(it->second.explain);
        }
    }
    return res;
}

const char *Defines::macroText_SHT(Elf64_Word val)
{
    return anyTextSlow(val, defines_SHT, &DefineInfo<>::name, "Unknown");
}

const char *Defines::commentText_SHT(Elf64_Word val)
{
    return anyTextSlow(val, defines_SHT, &DefineInfo<>::explain, "Unknown");
}

size_t Defines::commentText_PF(Elf64_Word val, char *dst)
{
    static char buf[] = "rwx, OS-specific, Processor-specific";
    memset(buf, 0, sizeof(buf));
    memset(buf, '-', 3);
    if((val & PF_R)) {
        buf[0] = 'r';
    }
    if((val & PF_W)) {
        buf[1] = 'w';
    }
    if((val & PF_X)) {
        buf[2] = 'x';
    }
    if((val & PF_MASKOS)) {
        strncat(buf, ", OS-specific", sizeof(buf)-strlen(buf)-1);
    }
    if((val & PF_MASKPROC)) {
        strncat(buf, ", Processor-specific", sizeof(buf)-strlen(buf)-1);
    }
    if(dst) {
        strcpy(dst, buf);
    }
    return strlen(buf);
}

END_PLUG_NAMESPACE
