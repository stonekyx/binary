#include <iostream>

#include "CodeBlock.h"

using namespace std;
USE_BIN_NAMESPACE(backend);
BEGIN_PLUG_NAMESPACE(flow)

CodeBlock::CodeBlock(Elf64_Addr addr, Elf64_Addr raddr) :
    _startAddr(addr), _endAddr(addr), _reprAddr(raddr)
{
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
    _repr += QString("0x%1:\t%2\t%3\\l")
        .arg(_reprAddr, 0, 16).arg(inst.comm).arg(inst.args.join(", "));
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

const QString &CodeBlock::getRepr() const
{
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
