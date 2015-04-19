#ifndef PLUGIN_FLOW_CODE_BLOCK_H
#define PLUGIN_FLOW_CODE_BLOCK_H

#include "backend/File.h"
#include <QtCore/QString>

#include "common.h"

BEGIN_PLUG_NAMESPACE(flow)

class CodeBlock {
public:
    CodeBlock(Elf64_Addr);
    bool addInst(const BIN_NAMESPACE(backend)::File::DisasmInstInfo &);
    const QString &getRepr() const;
    Elf64_Addr getJumpTarget() const;
private:
    Elf64_Addr _endAddr;
    Elf64_Addr _reprAddr;
    QString _repr;
    Elf64_Addr _jumpTarget;
};

END_PLUG_NAMESPACE

#endif
