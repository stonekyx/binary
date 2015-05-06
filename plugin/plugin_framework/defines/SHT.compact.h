/*
 * Copyright 2015 KANG Yuxuan
 *
 * SHT.compact.h by KANG Yuxuan <stonekyx@gmail.com>
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

BINARY_PLUGIN_VALUE_TYPE, Elf64_Word
BINARY_PLUGIN_NAMESPACE, plugin_framework
BINARY_PLUGIN_OBJECTNAME, defines_SHT

SHT_NULL, "Section header table entry unused"
SHT_PROGBITS, "Program data"
SHT_SYMTAB, "Symbol table"
SHT_STRTAB, "String table"
SHT_RELA, "Relocation entries with addends"
SHT_HASH, "Symbol hash table"
SHT_DYNAMIC, "Dynamic linking information"
SHT_NOTE, "Notes"
SHT_NOBITS, "Program space with no data (bss)"
SHT_REL, "Relocation entries, no addends"
SHT_SHLIB, "Reserved"
SHT_DYNSYM, "Dynamic linker symbol table"
SHT_INIT_ARRAY, "Array of constructors"
SHT_FINI_ARRAY, "Array of destructors"
SHT_PREINIT_ARRAY, "Array of pre-constructors"
SHT_GROUP, "Section group"
SHT_SYMTAB_SHNDX, "Extended section indeces"
SHT_NUM, "Number of defined types."
SHT_LOOS, SHT_HIOS, "OS-specific."
SHT_GNU_ATTRIBUTES, "Object attributes."
SHT_GNU_HASH, "GNU-style hash table."
SHT_GNU_LIBLIST, "Prelink library list"
SHT_CHECKSUM, "Checksum for DSO content."
SHT_LOSUNW, SHT_HISUNW, "Sun-specific"
SHT_SUNW_move
SHT_SUNW_COMDAT
SHT_SUNW_syminfo
SHT_GNU_verdef, "Version definition section."
SHT_GNU_verneed, "Version needs section."
SHT_GNU_versym, "Version symbol table."
SHT_LOPROC, SHT_HIPROC, "Processor-specific"
SHT_LOUSER, SHT_HIUSER, "Application-specific"
