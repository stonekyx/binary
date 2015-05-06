/*
 * Copyright 2015 KANG Yuxuan
 *
 * DisasmTree.cpp by KANG Yuxuan <stonekyx@gmail.com>
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
