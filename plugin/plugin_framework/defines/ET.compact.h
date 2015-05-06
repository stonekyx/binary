/*
 * Copyright 2015 KANG Yuxuan
 *
 * ET.compact.h by KANG Yuxuan <stonekyx@gmail.com>
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

BINARY_PLUGIN_VALUE_TYPE, Elf64_Half
BINARY_PLUGIN_NAMESPACE, plugin_framework
BINARY_PLUGIN_OBJECTNAME, defines_ET

/* Legal values for e_type (object file type). */

ET_NONE, "No file type"
ET_REL, "Relocatable file"
ET_EXEC, "Executable file"
ET_DYN, "Shared object file"
ET_CORE, "Core file"
ET_NUM, "Number of defined types"
ET_LOOS, ET_HIOS, "OS-specific range"
ET_LOPROC, ET_HIPROC, "Processor-specific range"
