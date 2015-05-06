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
#include "backend/Backend.h"
#include "frontend/PluginManager.h"
#include "backend/ConvertAddr.h"
#include "InstData.h"
#include "DisasmMetadata.h"
#include "ui_MainWindow.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(disasm)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MainWindow(), plugin, param, parent),
    _infoModel(NULL),
    _loadWorker(NULL),
    _ui(dynamic_cast<Ui::MainWindow*>(MWTreeView::_ui))
{
    QObject::connect(_ui, SIGNAL(signalStopDisasm()),
            this, SLOT(resetWorker()));
    QObject::connect(_ui, SIGNAL(signalRefreshDisasm()),
            this, SLOT(updateInfo()));
    QObject::connect(_ui, SIGNAL(signalRangeChange(Elf64_Off, Elf64_Off)),
            this, SLOT(setRange(Elf64_Off, Elf64_Off)));
    QObject::connect(_ui, SIGNAL(signalVaddrRangeChange(Elf64_Addr, Elf64_Addr)),
            this, SLOT(setVaddrRange(Elf64_Addr, Elf64_Addr)));
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    _useRange = _useVRange = false;
    _begin = _end = 0;
    if(param.find("vBegin") != param.end() &&
            param.find("vEnd") != param.end())
    {
        _useRange = true;
        _useVRange = true;
        _begin = QString(param["vBegin"].c_str()).toULong();
        _end = QString(param["vEnd"].c_str()).toULong();
    }
    QFont font = _ui->infoTree->font();
    font.setFamily("Courier");
    font.setPointSize(9);
    _ui->infoTree->setFont(font);
    updateInfo();
}

MainWindow::~MainWindow()
{
    resetWorker();
    if(_infoModel->metadata().isValid()) {
        delete _infoModel->metadata().value<DisasmMetadata*>();
    }
    if(_infoModel) {
        delete _infoModel;
    }
}

void MainWindow::updateInfo(File *file)
{
    resetWorker();
    if(!_ui->switchMode(file)) {
        return;
    }
    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 5, NULL);
    _ui->infoTree->setModel(_infoModel);

    Elf64_Ehdr ehdr;
    if(!file->getEhdr(&ehdr)) {
        return;
    }
    ConvertAddr convertAddr(file);

    if(_useRange) {
        if(_useVRange) {
            Elf64_Off fBegin=0, fEnd=0;
            if(ehdr.e_type == ET_REL) {
                convertAddr.secOffToFileOff(fBegin, _scnIndex, _begin);
                convertAddr.secOffToFileOff(fEnd, _scnIndex, _end);
            } else {
                convertAddr.vaddrToFileOff(fBegin, _begin);
                convertAddr.vaddrToFileOff(fEnd, _end);
            }
            _begin = fBegin;
            _end = fEnd;
            _useVRange = false;
        }
        _ui->setRange(_begin, _end);
        Elf64_Addr _vBegin=0, _vEnd=0;
        convertAddr.fileOffToVaddr(_vBegin, _begin);
        convertAddr.fileOffToVaddr(_vEnd, _end);
        _ui->setVaddrRange(_vBegin, _vEnd);
        if(_begin < _end) {
            _loadWorker = new LoadWorker(_begin, _end, file, _infoModel);
        }
    } else if(_scnIndex != 0) {
        Elf64_Shdr shdr;
        if(!file->getShdr(_scnIndex, &shdr)) {
            QMessageBox::critical(this, tr("Error"),
                    tr("Can't find specified section!"));
            close();
            return;
        }
        _ui->setRange(shdr.sh_offset, shdr.sh_offset + shdr.sh_size);
        _ui->setVaddrRange(shdr.sh_addr, shdr.sh_addr + shdr.sh_size);
        _loadWorker = new LoadWorker(
                shdr.sh_offset, shdr.sh_offset + shdr.sh_size,
                file, _infoModel);
    } else {
        _loadWorker = new LoadWorker(file, _infoModel);
    }

    if(_loadWorker) {
        QObject::connect(_loadWorker, SIGNAL(symbolStarted(QModelIndex)),
                this, SLOT(spanFirstColumn(QModelIndex)));
        QObject::connect(_loadWorker, SIGNAL(started()),
                _ui, SLOT(changeStatus()));
        QObject::connect(_loadWorker, SIGNAL(finished()),
                _ui, SLOT(changeStatus()));
        QObject::connect(_loadWorker, SIGNAL(terminated()),
                _ui, SLOT(changeStatus()));
        QObject::connect(
                _loadWorker, SIGNAL(timerStopped(double, double)),
                _ui, SLOT(showLoadTime(double, double)));
        _loadWorker->start();
    }
}

void MainWindow::spanFirstColumn(QModelIndex index)
{
    _ui->infoTree->setFirstColumnSpanned(index.row(), index.parent(),
            true);
}

void MainWindow::resetWorker()
{
    if(!_loadWorker) {
        return;
    }
    _loadWorker->terminate();
    _loadWorker->wait(1000);
    _loadWorker->deleteLater();
    _loadWorker = NULL;
}

void MainWindow::setRange(Elf64_Off begin, Elf64_Off end)
{
    if(!_useRange || begin != _begin || end != _end) {
        _useRange = true;
        _useVRange = false;
        _begin = begin;
        _end = end;
        updateInfo();
    }
}

void MainWindow::setVaddrRange(Elf64_Addr begin, Elf64_Addr end)
{
    Elf64_Off fBegin = 0, fEnd = 0;
    ConvertAddr convertAddr(_plugin->manager->getBackend()->getFile());
    convertAddr.vaddrToFileOff(fBegin, begin);
    convertAddr.vaddrToFileOff(fEnd, end);
    if(!_useRange || fBegin != _begin || fEnd != _end) {
        _useRange = true;
        _useVRange = false;
        _begin = fBegin;
        _end = fEnd;
        updateInfo();
    }
}

void MainWindow::openScnData()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    if(!action) {
        return;
    }
    const InstData &instData = action->data().value<InstData>();
    size_t scnIdx;
    Elf64_Off scnOff;
    ConvertAddr convertAddr(_plugin->manager->getBackend()->getFile());
    convertAddr.vaddrToSecOff(scnIdx, scnOff, instData.d.vaddr.addr);
    map<string, string> param;
    param["scnIndex"] = QString::number(scnIdx).toUtf8().constData();
    param["scnOffset"] = QString::number(scnOff).toUtf8().constData();
    BIN_NAMESPACE(frontend)::Plugin *plugin =
        _plugin->manager->getPlugin("ScnData");
    if(plugin) {
        plugin->createView(param);
    }
}

void MainWindow::jumpOrOpenSym()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    if(!action) {
        return;
    }
    const InstData &instData = action->data().value<InstData>();

    //---------get file
    File *file = _plugin->manager->getBackend()->getFile();
    if(!file) return;

    //---------convert to file offset
    Elf64_Off fileOff;
    ConvertAddr convertAddr(file);
    if(!convertAddr.vaddrToFileOff(fileOff, instData.d.symbol.addr)) return;

    //---------get and check symbol
    Elf64_Sym sym;
    if(!file->getLastSymDataByFileOff(fileOff, &sym)) return;
    Elf64_Off symFileOff;
    if(!file->getSymFileOff(&symFileOff, &sym) || symFileOff != fileOff)
        return;

    //---------check current window
    DisasmMetadata *dm = _infoModel->metadata().value<DisasmMetadata*>();
    if(dm && dm->instMap.find(fileOff) != dm->instMap.end()) {
        QModelIndex target = dm->instMap[fileOff];
        QModelIndex symSpan = target.sibling(target.row()-1, 0);
        if(symSpan.isValid() && _ui->infoTree->isFirstColumnSpanned(symSpan.row(), symSpan.parent())) {
            target = symSpan;
        }
        _ui->infoTree->scrollTo(target, QAbstractItemView::PositionAtCenter);
        _ui->infoTree->selectionModel()->select(target,
                QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        return;
    }

    //---------assemble param
    map<string, string> param;
    param["vBegin"] = QString::number(sym.st_value).toUtf8().constData();
    param["vEnd"] = QString::number(sym.st_value+sym.st_size).toUtf8().constData();
    param["scnIndex"] = QString::number(sym.st_shndx).toUtf8().constData();
    BIN_NAMESPACE(frontend)::Plugin *plugin =
        _plugin->manager->getPlugin("Disasm");
    if(plugin) {
        plugin->createView(param);
    }
}

void MainWindow::openReloc()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    if(!action) {
        return;
    }
    const InstData &instData = action->data().value<InstData>();

    //---------get file
    File *file = _plugin->manager->getBackend()->getFile();
    if(!file) return;

    //---------assemble param
    map<string, string> param;
    param["relocStart"] = QString::number(instData.d.reloc.start).toUtf8().constData();
    param["relocEnd"] = QString::number(instData.d.reloc.end).toUtf8().constData();
    BIN_NAMESPACE(frontend)::Plugin *plugin =
        _plugin->manager->getPlugin("Reloc");
    if(plugin) {
        plugin->createView(param);
    }
}

void MainWindow::openFlowDgm()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    if(!action) {
        return;
    }
    const InstData &instData = action->data().value<InstData>();

    //---------get file
    File *file = _plugin->manager->getBackend()->getFile();
    if(!file) return;

    //---------get and check symbol
    Elf64_Sym sym;
    if(!file->getLastSymDataByFileOff(instData.d.start.off, &sym)) return;
    Elf64_Off symFileOff;
    if(!file->getSymFileOff(&symFileOff, &sym) || symFileOff != instData.d.start.off)
        return;

    //---------assemble param
    map<string, string> param;
    param["vBegin"] = QString::number(sym.st_value).toUtf8().constData();
    param["vEnd"] = QString::number(sym.st_value+sym.st_size).toUtf8().constData();
    param["scnIndex"] = QString::number(sym.st_shndx).toUtf8().constData();
    BIN_NAMESPACE(frontend)::Plugin *plugin =
        _plugin->manager->getPlugin("Flow");
    if(plugin) {
        plugin->createView(param);
    }
}

END_PLUG_NAMESPACE
