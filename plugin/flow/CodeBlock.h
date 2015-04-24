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