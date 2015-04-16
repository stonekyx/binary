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
