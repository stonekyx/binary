/*
 * Copyright 2015 KANG Yuxuan
 *
 * CodeBlock.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_FLOW_CODE_BLOCK_H
#define PLUGIN_FLOW_CODE_BLOCK_H

#include "backend/File.h"
#include <QtCore/QString>

#include "common.h"

BEGIN_PLUG_NAMESPACE(flow)

class CodeBlock {
public:
    CodeBlock(Elf64_Addr, Elf64_Addr raddr = 0);
    bool addInst(const BIN_NAMESPACE(backend)::File::DisasmInstInfo &);
    const QString &getRepr();
    Elf64_Addr getJumpTarget() const;
    bool getJumpCond() const;
    Elf64_Addr getStartAddr() const;
private:
    QStringList _addrs, _comms, _args;
    Elf64_Addr _startAddr;
    Elf64_Addr _endAddr;
    Elf64_Addr _reprAddr;
    QString _repr;
    Elf64_Addr _jumpTarget;
    bool _jumpCond;
    bool _changed;
};

END_PLUG_NAMESPACE

#endif
