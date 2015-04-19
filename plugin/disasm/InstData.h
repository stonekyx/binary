#ifndef PLUGIN_DISASM_INST_DATA_H
#define PLUGIN_DISASM_INST_DATA_H

#include <elf.h>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>

#include "common.h"

BEGIN_PLUG_NAMESPACE(disasm)

class InstData {
public:
    union {
        struct { Elf64_Addr addr;       } vaddr;

        struct { Elf64_Addr addr;
                 Elf64_Off off;         } insym;

        struct { Elf64_Addr addr;
                 char *symName;         } symbol;

        struct { Elf64_Off start;
                 char *symName;
                 Elf64_Off end;         } reloc;

        struct { Elf64_Off off;
                 char *symName;         } start;
    } d;
    typedef enum {
        AT_NONE,
        AT_RELOC,
        AT_PLT,
        AT_SYMBOL,
        AT_VADDR,
        AT_VADDR_INSYM,
        AT_SYMBOL_START
    } AddrType;
    AddrType addrType;
    InstData();
    InstData(const InstData &);
    InstData &operator=(const InstData &);
    ~InstData();
};

END_PLUG_NAMESPACE

Q_DECLARE_METATYPE(binary::plugin::disasm::InstData)

#endif
