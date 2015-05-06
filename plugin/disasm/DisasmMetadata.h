/*
 * Copyright 2015 KANG Yuxuan
 *
 * DisasmMetadata.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_DISASM_DISASM_METADATA_H
#define PLUGIN_DISASM_DISASM_METADATA_H

#include <map>
#include <elf.h>
#include <QtCore/QModelIndex>
#include <QtCore/QMetaType>

#include "common.h"

BEGIN_PLUG_NAMESPACE(disasm)

class DisasmMetadata {
public:
    std::map<Elf64_Off, QModelIndex> instMap;
};

END_PLUG_NAMESPACE

Q_DECLARE_METATYPE(binary::plugin::disasm::DisasmMetadata*)

#endif
