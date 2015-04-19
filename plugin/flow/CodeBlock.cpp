#include <iostream>

#include "CodeBlock.h"

using namespace std;
USE_BIN_NAMESPACE(backend);
BEGIN_PLUG_NAMESPACE(flow)

CodeBlock::CodeBlock(Elf64_Addr addr) : _endAddr(addr), _reprAddr(0)
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
    if(!_repr.isEmpty()) { _repr += "\n"; }
    _repr += QString("0x%1:\t%2\t%3")
        .arg(_reprAddr).arg(inst.comm).arg(inst.args.join(", "));
    _endAddr += inst.size;
    _reprAddr += inst.size;
    if(inst.comm.startsWith("j")) {
        if(inst.comm.startsWith("jmp")) {
            _jumpTarget = _endAddr;
        } else {
            _jumpTarget = calcAddr(inst);
        }
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

END_PLUG_NAMESPACE
