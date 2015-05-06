/*
 * Copyright 2015 KANG Yuxuan
 *
 * InstData.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_DISASM_INST_DATA_H
#define PLUGIN_DISASM_INST_DATA_H

#include <elf.h>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>

#include "common.h"

BEGIN_PLUG_NAMESPACE(disasm)

class InstData {
public:
    union {
        struct { Elf64_Addr addr;       } vaddr;

        struct { Elf64_Addr addr;
                 Elf64_Off off;         } insym;

        struct { Elf64_Addr addr;
                 char *symName;         } symbol;

        struct { Elf64_Off start;
                 char *symName;
                 Elf64_Off end;         } reloc;

        struct { Elf64_Off off;
                 char *symName;         } start;
    } d;
    typedef enum {
        AT_NONE,
        AT_RELOC,
        AT_PLT,
        AT_SYMBOL,
        AT_VADDR,
        AT_VADDR_INSYM,
        AT_SYMBOL_START
    } AddrType;
    AddrType addrType;
    InstData();
    InstData(const InstData &);
    InstData &operator=(const InstData &);
    ~InstData();
};

END_PLUG_NAMESPACE

Q_DECLARE_METATYPE(binary::plugin::disasm::InstData)

#endif
