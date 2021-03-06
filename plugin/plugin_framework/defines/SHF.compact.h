/*
 * Copyright 2015 KANG Yuxuan
 *
 * SHF.compact.h by KANG Yuxuan <stonekyx@gmail.com>
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

BINARY_PLUGIN_VALUE_TYPE, Elf64_Xword
BINARY_PLUGIN_NAMESPACE, plugin_framework
BINARY_PLUGIN_OBJECTNAME, defines_SHF

SHF_WRITE, "Writable"
SHF_ALLOC, "Occupies memory during execution"
SHF_EXECINSTR, "Executable"
SHF_MERGE, "Might be merged"
SHF_STRINGS, "Contains nul-terminated strings"
SHF_INFO_LINK, "`sh_info' contains SHT index"
SHF_LINK_ORDER, "Preserve order after combining"
SHF_OS_NONCONFORMING, "Non-standard OS specific handling required"
SHF_GROUP, "Section is member of a group."
SHF_TLS, "Section hold thread-local data."
SHF_MASKOS, "OS-specific."
SHF_MASKPROC, "Processor-specific"
SHF_ORDERED, "Special ordering requirement (Solaris)."
SHF_EXCLUDE, "Section is excluded unless referenced or allocated (Solaris)."
