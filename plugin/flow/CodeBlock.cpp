/*
 * Copyright 2015 KANG Yuxuan
 *
 * CodeBlock.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <iostream>

#include "CodeBlock.h"

using namespace std;
USE_BIN_NAMESPACE(backend);
BEGIN_PLUG_NAMESPACE(flow)

CodeBlock::CodeBlock(Elf64_Addr addr, Elf64_Addr raddr) :
    _startAddr(addr), _endAddr(addr), _reprAddr(raddr)
{
    _jumpCond = false;
    _jumpTarget = _endAddr;
    _changed = false;
}

static Elf64_Addr calcAddr(const File::DisasmInstInfo &inst)
{
    if(inst.hasLabel) {
        return inst.label;
    }
    foreach(const QString &arg, inst.args) {
        bool ok;
        Elf64_Addr argAddr;
        if(arg.startsWith("$")) {
            argAddr = arg.right(arg.length()-1).toULong(&ok, 0);
        } else {
            argAddr = arg.toULong(&ok, 0);
        }
        if(ok) { return argAddr; }
    }
    cerr<<"Warning: instruction error!"<<endl;
    return 0;
}

bool CodeBlock::addInst(const File::DisasmInstInfo &inst)
{
    _addrs.push_back(QString("0x%1").arg(_reprAddr, 0, 16));
    _comms.push_back(inst.comm);
    _args.push_back(inst.args.join(", "));
    _changed = true;

    _endAddr += inst.size;
    _reprAddr += inst.size;
    _jumpCond = false;
    _jumpTarget = _endAddr;
    if(inst.comm.startsWith("j")) {
        _jumpCond = !inst.comm.startsWith("jmp");
        _jumpTarget = calcAddr(inst);
        return false;
    }
    return true;
}

const QString &CodeBlock::getRepr()
{
    if(!_changed) {
        return _repr;
    }
    int maxAddrLen = 0;
    int maxCommLen = 0;
    for(int i=0; i<_addrs.size(); i++) {
        if(maxAddrLen < _addrs[i].length()) {
            maxAddrLen = _addrs[i].length();
        }
        if(maxCommLen < _comms[i].length()) {
            maxCommLen = _comms[i].length();
        }
    }
    for(int i=0; i<_addrs.size(); i++) {
        _repr += QString("%1:%2 %3%4   %5\\l")
            .arg(_addrs[i])
            .arg(QString(" ").repeated(maxAddrLen - _addrs[i].length()))
            .arg(_comms[i])
            .arg(QString(" ").repeated(maxCommLen - _comms[i].length()))
            .arg(_args[i]);
    }
    _changed = false;
    return _repr;
}

Elf64_Addr CodeBlock::getJumpTarget() const
{
    return _jumpTarget;
}

bool CodeBlock::getJumpCond() const
{
    return _jumpCond;
}

Elf64_Addr CodeBlock::getStartAddr() const
{
    return _startAddr;
}

END_PLUG_NAMESPACE
