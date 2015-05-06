/*
 * Copyright 2015 KANG Yuxuan
 *
 * PT.compact.h by KANG Yuxuan <stonekyx@gmail.com>
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
BINARY_PLUGIN_OBJECTNAME, defines_PT

/* Legal values for p_type (segment type). */

PT_NULL, "Program header table entry unused"
PT_LOAD, "Loadable program segment"
PT_DYNAMIC, "Dynamic linking information"
PT_INTERP, "Program interpreter"
PT_NOTE, "Auxiliary information"
PT_SHLIB, "Reserved"
PT_PHDR, "Entry for header table itself"
PT_TLS, "Thread-local storage segment"
PT_NUM, "Number of defined types"
PT_LOOS, PT_HIOS, "OS-specific"
PT_GNU_EH_FRAME, "GCC .eh_frame_hdr segment"
PT_GNU_STACK, "Indicates stack executability"
PT_GNU_RELRO, "Read-only after relocation"
PT_LOSUNW, PT_HISUNW, "Sun specific"
PT_SUNWBSS, "Sun Specific segment"
PT_SUNWSTACK, "Stack segment"
PT_LOPROC, PT_HIPROC, "Processor-specific"
