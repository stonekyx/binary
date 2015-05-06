/*
 * Copyright 2015 KANG Yuxuan
 *
 * ConvertClass.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_CONVERT_CLASS_H
#define BACKEND_CONVERT_CLASS_H

#include "common.h"

BEGIN_BIN_NAMESPACE(backend)

class ConvertClass {
public:
    template<typename T>
    static void cvt(Elf64_Sym &dst, const T &src) {
        dst.st_name = src.st_name;
        dst.st_info = src.st_info;
        dst.st_other = src.st_other;
        dst.st_shndx = src.st_shndx;
        dst.st_value = src.st_value;
        dst.st_size = src.st_size;
    }
    template<typename T>
    static void cvt(Elf64_Ehdr &dst, const T &src) {
        dst.e_type = src.e_type;
        dst.e_machine = src.e_machine;
        dst.e_version = src.e_version;
        dst.e_entry = src.e_entry;
        dst.e_phoff = src.e_phoff;
        dst.e_shoff = src.e_shoff;
        dst.e_flags = src.e_flags;
        dst.e_ehsize = src.e_ehsize;
        dst.e_phentsize = src.e_phentsize;
        dst.e_phnum = src.e_phnum;
        dst.e_shentsize = src.e_shentsize;
        dst.e_shnum = src.e_shnum;
        dst.e_shstrndx = src.e_shstrndx;
    }
    template<typename T>
    static void cvt(Elf64_Phdr &dst, const T &src) {
        dst.p_type = src.p_type;
        dst.p_flags = src.p_flags;
        dst.p_offset = src.p_offset;
        dst.p_vaddr = src.p_vaddr;
        dst.p_paddr = src.p_paddr;
        dst.p_filesz = src.p_filesz;
        dst.p_memsz = src.p_memsz;
        dst.p_align = src.p_align;
    }
    template<typename T>
    static void cvt(Elf64_Shdr &dst, const T &src) {
        dst.sh_name = src.sh_name;
        dst.sh_type = src.sh_type;
        dst.sh_flags = src.sh_flags;
        dst.sh_addr = src.sh_addr;
        dst.sh_offset = src.sh_offset;
        dst.sh_size = src.sh_size;
        dst.sh_link = src.sh_link;
        dst.sh_info = src.sh_info;
        dst.sh_addralign = src.sh_addralign;
        dst.sh_entsize = src.sh_entsize;
    }
    template<typename T>
    static void cvt(Elf64_Syminfo &dst, const T &src) {
        dst.si_boundto = src.si_boundto;
        dst.si_flags = src.si_flags;
    }
    template<typename T>
    static void cvt(Elf64_Dyn &dst, const T &src) {
        dst.d_tag = src.d_tag;
        dst.d_un.d_val = src.d_un.d_val;
    }
    template<typename T>
    static void cvt(Elf64_Rel &dst, const T &src) {
        dst.r_offset = src.r_offset;
        dst.r_info = src.r_info;
    }
    template<typename T>
    static void cvt(Elf64_Rela &dst, const T &src) {
        dst.r_offset = src.r_offset;
        dst.r_info = src.r_info;
        dst.r_addend = src.r_addend;
    }
};

END_BIN_NAMESPACE

#endif
