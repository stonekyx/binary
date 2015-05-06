/*
 * Copyright 2015 KANG Yuxuan
 *
 * DT.compact.h by KANG Yuxuan <stonekyx@gmail.com>
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

BINARY_PLUGIN_VALUE_TYPE, Elf64_Sxword
BINARY_PLUGIN_NAMESPACE, plugin_framework
BINARY_PLUGIN_OBJECTNAME, defines_DT

DT_NULL, "Marks end of dynamic section"
DT_NEEDED, "Name of needed library"
DT_PLTRELSZ, "Size in bytes of PLT relocs"
DT_PLTGOT, "Processor defined value"
DT_HASH, "Address of symbol hash table"
DT_STRTAB, "Address of string table"
DT_SYMTAB, "Address of symbol table"
DT_RELA, "Address of Rela relocs"
DT_RELASZ, "Total size of Rela relocs"
DT_RELAENT, "Size of one Rela reloc"
DT_STRSZ, "Size of string table"
DT_SYMENT, "Size of one symbol table entry"
DT_INIT, "Address of init function"
DT_FINI, "Address of termination function"
DT_SONAME, "Name of shared object"
DT_RPATH, "Library search path (deprecated)"
DT_SYMBOLIC, "Start symbol search here"
DT_REL, "Address of Rel relocs"
DT_RELSZ, "Total size of Rel relocs"
DT_RELENT, "Size of one Rel reloc"
DT_PLTREL, "Type of reloc in PLT"
DT_DEBUG, "For debugging; unspecified"
DT_TEXTREL, "Reloc might modify .text"
DT_JMPREL, "Address of PLT relocs"
DT_BIND_NOW, "Process relocations of object"
DT_INIT_ARRAY, "Array with addresses of init fct"
DT_FINI_ARRAY, "Array with addresses of fini fct"
DT_INIT_ARRAYSZ, "Size in bytes of DT_INIT_ARRAY"
DT_FINI_ARRAYSZ, "Size in bytes of DT_FINI_ARRAY"
DT_RUNPATH, "Library search path"
DT_FLAGS, "Flags for the object being loaded"
DT_ENCODING, "Start of encoded range"
DT_PREINIT_ARRAY, "Array with addresses of preinit fct"
DT_PREINIT_ARRAYSZ, "size in bytes of DT_PREINIT_ARRAY"
/* DT_NUM, "Number used" */
DT_LOOS, DT_HIOS, "OS-specific"
DT_LOPROC, DT_HIPROC, "Processor-specific"
/* DT_PROCNUM, "Most used by any processor" */

/* DT_* entries which fall between DT_VALRNGHI & DT_VALRNGLO use the
   Dyn.d_un.d_val field of the Elf*_Dyn structure.  This follows Sun's
   approach. */
DT_VALRNGLO
DT_GNU_PRELINKED, "Prelinking timestamp"
DT_GNU_CONFLICTSZ, "Size of conflict section"
DT_GNU_LIBLISTSZ, "Size of library list"
DT_CHECKSUM
DT_PLTPADSZ
DT_MOVEENT
DT_MOVESZ
DT_FEATURE_1, "Feature selection (DTF_*)."
DT_POSFLAG_1, "Flags for DT_* entries, effecting the following DT_* entry."
DT_SYMINSZ, "Size of syminfo table (in bytes)"
DT_SYMINENT, "Entry size of syminfo"
DT_VALRNGHI
/* DT_VALTAGIDX(tag), "Reverse order!" */
/* DT_VALNUM 12 */

/* DT_* entries which fall between DT_ADDRRNGHI & DT_ADDRRNGLO use the
   Dyn.d_un.d_ptr field of the Elf*_Dyn structure.

   If any adjustment is made to the ELF object after it has been
   built these entries will need to be adjusted. */
DT_ADDRRNGLO
DT_GNU_HASH, "GNU-style hash table."
DT_TLSDESC_PLT
DT_TLSDESC_GOT
DT_GNU_CONFLICT, "Start of conflict section"
DT_GNU_LIBLIST, "Library list"
DT_CONFIG, "Configuration information."
DT_DEPAUDIT, "Dependency auditing."
DT_AUDIT, "Object auditing."
DT_PLTPAD, "PLT padding."
DT_MOVETAB, "Move table."
DT_SYMINFO, "Syminfo table."
DT_ADDRRNGHI
/* DT_ADDRTAGIDX(tag), "Reverse order!" */
/* DT_ADDRNUM 11 */

/* The versioning entry types.  The next are defined as part of the
   GNU extension. */
DT_VERSYM

DT_RELACOUNT
DT_RELCOUNT

/* These were chosen by Sun. */
DT_FLAGS_1, "State flags, see DF_1_* below."
DT_VERDEF, "Address of version definition table"
DT_VERDEFNUM, "Number of version definitions"
DT_VERNEED, "Address of table with needed versions"
DT_VERNEEDNUM, "Number of needed versions"
/* DT_VERSIONTAGIDX(tag), "Reverse order!" */
/* DT_VERSIONTAGNUM 16 */

/* Sun added these machine-independent extensions in the "processor-specific"
   range.  Be compatible. */
DT_AUXILIARY, "Shared object to load before self"
DT_FILTER, "Shared object to get values from"
/* DT_EXTRATAGIDX(tag) ((Elf32_Word)-((Elf32_Sword) (tag) <<1>>1)-1) */
/* DT_EXTRANUM 3 */
