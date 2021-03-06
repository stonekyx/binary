/*
 * Copyright 2015 KANG Yuxuan
 *
 * STT.compact.h by KANG Yuxuan <stonekyx@gmail.com>
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

BINARY_PLUGIN_VALUE_TYPE, int
BINARY_PLUGIN_NAMESPACE, plugin_framework
BINARY_PLUGIN_OBJECTNAME, defines_STT

STT_NOTYPE, "Symbol type is unspecified"
STT_OBJECT, "Symbol is a data object"
STT_FUNC, "Symbol is a code object"
STT_SECTION, "Symbol associated with a section"
STT_FILE, "Symbol's name is file name"
STT_COMMON, "Symbol is a common data object"
STT_TLS, "Symbol is thread-local data object"
STT_NUM, "Number of defined types."
STT_LOOS, STT_HIOS, "OS-specific"
STT_GNU_IFUNC, "Symbol is indirect code object"
STT_LOPROC, STT_HIPROC, "Processor-specific"
