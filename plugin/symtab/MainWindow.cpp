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
#include <QtGui/QMessageBox>
#include <QtGui/QTextDocument>

#include "backend/File.h"
#include "backend/Backend.h"
#include "frontend/PluginManager.h"
#include "backend/ConvertClass.h"
#include "ui_MWTreeView.h"
#include "DemangleWrap.h"
#include "Defines.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(symtab)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginSymTabMainWindow", "Symbol table"), plugin, param, parent),
    _infoModel(NULL)
{
    ctxMenuTreeView();
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    updateInfo();
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

static size_t symTabIndex(File *file, size_t knownIdx, Elf64_Shdr *dst)
{
    if(knownIdx != 0) {
        if(!file->getShdr(knownIdx, dst) ||
                (dst->sh_type != SHT_SYMTAB &&
                 dst->sh_type != SHT_DYNSYM))
        {
            return 0;
        }
        return knownIdx;
    }
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return 0;
    }
    for(size_t i=0; i<shdrNum; i++) {
        if(!file->getShdr(i, dst)) {
            continue;
        }
        if(dst->sh_type == SHT_SYMTAB) {
            return i;
        }
    }
    return 0;
}

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    switch(file->getKind()) {
    case File::KIND_AR:
        updateArInfo(file);
        break;
    case File::KIND_ELF:
        updateElfInfo(file);
        break;
    default:
        break;
    }
}

void MainWindow::openDisasm()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    if(!action) {
        return;
    }
    QModelIndex index = _ui->infoTree->indexAt(action->data().toPoint());
    while(index.parent().isValid()) {
        index = index.parent();
    }
    if(index.model()->rowCount(index) <= 2) {
        return; //ar
    }
    QString shndx = index.child(3, 1).data().toString();
    if(shndx.indexOf(" ") != -1) {
        shndx = shndx.left(shndx.indexOf(" "));
    } else {
        shndx = QString();
    }
    bool ok;
    Elf64_Addr vaddr =
        index.child(4, 1).data().toString().toULong(&ok, 0);
    if(!ok) { return; }
    Elf64_Xword size =
        index.child(5, 1).data().toString().toULong(&ok, 0);
    if(!ok) { return; }

    map<string, string> param;
    param["vBegin"] = QString::number(vaddr).toUtf8().constData();
    param["vEnd"] = QString::number(vaddr+size).toUtf8().constData();
    if(!shndx.isEmpty()) {
        param["scnIndex"] = shndx.toUtf8().constData();
    }
    BIN_NAMESPACE(frontend)::Plugin *plugin =
        _plugin->manager->getPlugin("Disasm");
    if(plugin) {
        plugin->createView(param);
    }
}

void MainWindow::updateArInfo(File *file)
{
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);

    size_t arsymNum = file->getArsymNum();
    for(size_t i=0; i<arsymNum; i++) {
        Arsym arsym = file->getArsym(i);
        _infoModel->buildMore(QString("Entry %1\t%2")
                .arg(i)
                .arg(arsym.name()));
        QString objName;
        Arhdr arhdr = file->getArhdrByOffset(arsym.off());
        if(arhdr.name()) {
            objName = QString(" (%1)").arg(arhdr.name());
        }
        _infoModel->buildMore(QString("\tOffset\t0x%1%2")
                .arg(arsym.off(), 0, 16)
                .arg(objName));
        _infoModel->buildMore(QString("\tHash\t0x%1")
                .arg(arsym.hash(), 0, 16));
    }

    _ui->infoTree->setModel(_infoModel);
}

void MainWindow::updateElfInfo(File *file)
{
    bool useShdr = false;
    bool usePhdr = false;
    char *dynSymRaw = NULL, *dynStrRaw = NULL;
    Elf64_Shdr shdr;
    size_t found;
    if((found = symTabIndex(file, _scnIndex, &shdr))) {
        useShdr = true;
    } else if((dynSymRaw = file->findDynTag(DT_SYMTAB))) {
        dynStrRaw = file->findDynTag(DT_STRTAB);
        usePhdr = true;
    }
    if(!useShdr && !usePhdr) {
        QMessageBox::critical(this, tr("Error"),
                tr("No symbol table found or not a valid symbol table."));
        close();
        return;
    }
    _scnIndex = found;

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);

    size_t symCount;
    if(useShdr) {
        symCount = shdr.sh_size / shdr.sh_entsize;
    } else {
        symCount = file->detectDynSymCnt();
    }
    for(size_t i=0; i<symCount; i++) {
        Elf64_Sym sym;
        if(useShdr) {
            if(!file->getSym(_scnIndex, i, &sym)) {
                continue;
            }
        } else {
            if(file->getClass() == ELFCLASS32) {
                ConvertClass::cvt(sym,
                        *(Elf32_Sym*)(dynSymRaw+i*sizeof(Elf32_Sym)));
            } else {
                ConvertClass::cvt(sym,
                        *(Elf64_Sym*)(dynSymRaw+i*sizeof(Elf64_Sym)));
            }
        }

        const char *symName;
        if(useShdr) {
            symName = file->getStrPtr(shdr.sh_link, sym.st_name);
        } else {
            symName = dynStrRaw ? dynStrRaw + sym.st_name : NULL;
        }
        char *demangle = cplus_demangle(symName);
        _infoModel->buildMore(QString("Entry %1\t%2\x1f<div>%3</div>")
                .arg(i)
                .arg(symName)
                .arg(Qt::escape(demangle)));
        free(demangle);
        _infoModel->buildMore(QString("\tName offset\t0x%1")
                .arg(sym.st_name, 0, 16));
        _infoModel->buildMore(QString("\tInfo\t0x%1")
                .arg(sym.st_info, 0, 16));
        _infoModel->buildMore(QString("\t\tBind\t%1")
                .arg(Defines::commentText(ELF64_ST_BIND(sym.st_info), defines_STB)));
        _infoModel->buildMore(QString("\t\tType\t%1")
                .arg(Defines::commentText(ELF64_ST_TYPE(sym.st_info), defines_STT)));
        _infoModel->buildMore(QString("\tVisibility\t%1")
                .arg(Defines::commentText(ELF64_ST_VISIBILITY(sym.st_other), defines_STV)));
        Elf64_Shdr shdr;
        QString scnName;
        if(useShdr &&
                sym.st_shndx && file->getShdr(sym.st_shndx, &shdr) &&
                !(scnName=file->getScnName(&shdr)).isEmpty())
        {
            scnName.prepend(" (");
            scnName.append(")");
        }
        _infoModel->buildMore(QString("\tSection index\t%1%2")
                .arg(sym.st_shndx).arg(scnName));
        _infoModel->buildMore(QString("\tValue\t0x%1")
                .arg(sym.st_value, 0, 16));
        _infoModel->buildMore(QString("\tSize\t0x%1")
                .arg(sym.st_size, 0, 16));
        if(ELF64_ST_TYPE(sym.st_info) == STT_FILE ||
                ELF64_ST_TYPE(sym.st_info) == STT_SECTION ||
                sym.st_shndx != 0)
        {
            continue;
        }
        QString symNameCooked(symName);
        if(symNameCooked.contains("@")) {
            symNameCooked.replace(QRegExp("@.*"), "");
        }
        const char *libName =
            file->queryDynSymDeps(symNameCooked.toUtf8().constData(),
                    NULL);
        if(!libName) {
            if(ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
                libName = "Not found";
            } else {
                libName = "Not found!!!";
            }
        }
        _infoModel->buildMore(QString("\tLibrary\t%1")
                .arg(libName));
    }
    _ui->infoTree->setModel(_infoModel);
}

void MainWindow::ctxMenuTreeView()
{
    QAction *actionDisasm = _ui->ctxMenu->addAction(
            tr("Disassemble"), this, SLOT(openDisasm()));
    actionDisasm->setParent(_ui->ctxMenu);
}

END_PLUG_NAMESPACE
