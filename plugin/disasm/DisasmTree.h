#ifndef PLUGIN_DISASM_DISASM_TREE_H
#define PLUGIN_DISASM_DISASM_TREE_H

#include <QtGui/QTreeView>

#include "common.h"

BEGIN_PLUG_NAMESPACE(disasm)

class DisasmTree;

END_PLUG_NAMESPACE

class binary::plugin::disasm::DisasmTree : public QTreeView {
public:
    DisasmTree(QWidget * = NULL);
protected:
    virtual void mouseMoveEvent(QMouseEvent *);
private:
    Elf64_Off _begin, _end;
};

#endif
