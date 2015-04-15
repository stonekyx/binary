#ifndef PLUGIN_DISASM_INST_DATA_H
#define PLUGIN_DISASM_INST_DATA_H

#include <elf.h>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>

#include "common.h"

BEGIN_PLUG_NAMESPACE(disasm)

class InstData {
public:
    QString comm;
    QStringList args;
    Elf64_Addr addr;
    Elf64_Addr addrRelocStart, addrRelocEnd;
    QStringList labels;
    typedef enum {
        AT_NONE,
        AT_RELOC,
        AT_PLT,
        AT_SYMBOL,
        AT_VADDR,
        AT_VADDR_INSYM
    } AddrType;
    AddrType addrType;
    QString symName;
};

END_PLUG_NAMESPACE

Q_DECLARE_METATYPE(binary::plugin::disasm::InstData)

#endif
