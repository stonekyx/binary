/*
 * Copyright 2015 KANG Yuxuan
 *
 * MainWindow.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <elf.h>

#include "backend/File.h"
#include "ui_MWTreeView.h"
#include "DemangleWrap.h"
#include "Defines.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(reloc)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginRelocMainWindow", "Relocations"), plugin, param, parent),
    _infoModel(NULL)
{
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    _setFocus = false;
    if(param.find("relocStart") != param.end() &&
            param.find("relocEnd") != param.end())
    {
        _setFocus = true;
        _relocStart = QString(param["relocStart"].c_str()).toULong();
        _relocEnd = QString(param["relocEnd"].c_str()).toULong();
    }
    updateInfo();
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2, NULL);
    scanShdr(file);
    _ui->infoTree->setModel(_infoModel);
    if(_setFocus && _focus.isValid()) {
        _ui->infoTree->scrollTo(_focus, QAbstractItemView::PositionAtCenter);
        _ui->infoTree->selectionModel()->select(_focus,
                QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
}

bool MainWindow::scanShdr(File *file)
{
    size_t shdrNum;
    Elf64_Ehdr ehdr;
    if(file->getShdrNum(&shdrNum) != 0 || !file->getEhdr(&ehdr)) {
        return false;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!file->getShdr(i, &shdr)) {
            continue;
        }
        if(shdr.sh_type != SHT_REL && shdr.sh_type != SHT_RELA) {
            continue;
        }
        _infoModel->buildMore(QString("Section\t%1")
                .arg(file->getScnName(&shdr)));
        for(size_t ent=0; ent<shdr.sh_size/shdr.sh_entsize; ent++) {
            union {
                Elf64_Rel rel;
                Elf64_Rela rela;
            } rel;
            if(shdr.sh_type == SHT_REL) {
                if(!file->getRel(i, ent, &rel.rel)) {
                    continue;
                }
            } else if(shdr.sh_type == SHT_RELA) {
                if(!file->getRela(i, ent, &rel.rela)) {
                    continue;
                }
            } else {
                continue;
            }
            Elf64_Sym sym;
            if(!file->getSym(shdr.sh_link,
                        ELF64_R_SYM(rel.rel.r_info), &sym))
            {
                continue;
            }
            Elf64_Shdr symShdr;
            if(!file->getShdr(shdr.sh_link, &symShdr)) {
                continue;
            }
            const char *symName =
                file->getStrPtr(symShdr.sh_link, sym.st_name);
            char *demangled = cplus_demangle(symName);
            _infoModel->buildMore(QString("\tSymbol\t%1\x1f%2")
                    .arg(symName)
                    .arg(demangled));
            free(demangled);

            QModelIndex inserted = _infoModel->buildMore(
                    QString("\t\tOffset\t0x%1")
                    .arg(rel.rel.r_offset, 0, 16));

            ExpandDefine<Elf64_Xword> *defines_R =
                Defines::commentText(ehdr.e_machine, defines_EM_R_mapping);
            Elf64_Xword r_type = ELF64_R_TYPE(rel.rel.r_info);
            QString macro = Defines::macroText(r_type, *defines_R);
            QString comment = Defines::commentText(r_type, *defines_R);
            _infoModel->buildMore(QString("\t\tType\t%1 (%2)")
                    .arg(ELF64_R_TYPE(rel.rel.r_info))
                    .arg(macro!=comment? macro + ": " + comment: macro));

            if(shdr.sh_type == SHT_RELA) {
                _infoModel->buildMore(QString("\t\tAddend\t%1")
                        .arg(rel.rela.r_addend));
            }
            _infoModel->buildMore(QString("\t\tLibrary\t%1")
                    .arg(file->queryDynSymDeps(symName, NULL)));

            Elf64_Off symFileOff;
            sym.st_value = rel.rel.r_offset;
            sym.st_shndx = shdr.sh_info;
            if(file->getSymFileOff(&symFileOff, &sym) &&
                    symFileOff >= _relocStart &&
                    symFileOff < _relocEnd)
            {
                _focus = inserted;
            }
        }
    }
    return true;
}

END_PLUG_NAMESPACE
