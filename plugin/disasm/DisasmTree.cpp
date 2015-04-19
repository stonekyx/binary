#include <QtGui/QMouseEvent>

#include "InfoModel.h"
#include "DisasmMetadata.h"
#include "InstData.h"

#include "DisasmTree.h"

USE_PLUG_NAMESPACE(plugin_framework);
BEGIN_PLUG_NAMESPACE(disasm)

DisasmTree::DisasmTree(QWidget *parent) : QTreeView(parent)
{
}

void DisasmTree::mouseMoveEvent(QMouseEvent *e)
{
    QTreeView::mouseMoveEvent(e);

    //----------select index under cursor
    QModelIndex idx = indexAt(e->pos());
    clearSelection();
    setCurrentIndex(idx);

    //----------check instruction type
    const InstData &instData = idx.data(Qt::UserRole).value<InstData>();
    if(instData.addrType != InstData::AT_VADDR_INSYM) {
        return;
    }

    //----------get metadata from model
    const InfoModel *infoModel = dynamic_cast<const InfoModel*>(idx.model());
    if(!infoModel) {
        return;
    }
    DisasmMetadata *dm = infoModel->metadata().value<DisasmMetadata*>();

    //----------select target
    if(dm->instMap.find(instData.d.insym.off) == dm->instMap.end()) {
        return;
    }
    setSelection(visualRect(dm->instMap[instData.d.insym.off]),
            QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

END_PLUG_NAMESPACE
