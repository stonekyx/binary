/*
 * Copyright 2015 KANG Yuxuan
 *
 * LoadWorker.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include "InstData.h"
#include "DisasmMetadata.h"

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "backend/ConvertAddr.h"
#include "DemangleWrap.h"
#include "LoadWorker.h"

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(disasm)

LoadWorker::LoadWorker(File *file, InfoModel *im, QObject *parent) :
    QThread(parent), _file(file), _infoModel(im)
{
    _restricted = false;
    file->registerWatcher(this);
    QObject::connect(file, SIGNAL(aboutToBeDestroyed()),
            this, SLOT(terminate()));
}

LoadWorker::LoadWorker(size_t begin, size_t end,
        File *file, InfoModel *im, QObject *parent) :
    QThread(parent), _file(file), _infoModel(im), _begin(begin), _end(end)
{
    _restricted = true;
    file->registerWatcher(this);
    QObject::connect(file, SIGNAL(aboutToBeDestroyed()),
            this, SLOT(terminate()));
}

static QString getTooltip(const QString &orig)
{
    char *demangle = cplus_demangle(orig.toUtf8().constData());
    QString res(demangle);
    free(demangle);
    return res;
}

static QString getTooltip(Elf64_Addr addr, const File::DisasmCBInfo &info)
{
    char *buf = info.convertAddr->vaddrToSecOffStrWithData(addr, 50);
    QString res(buf);
    free(buf);
    return res;
}

static bool sameSymbol(Elf64_Addr addr, const File::DisasmCBInfo &info)
{
    Elf64_Sym lastSym;
    if(!info.file->getLastSymDataByFileOff(
            (char*)info.last - info.file->getRawData(0), &lastSym))
    {
        return false;
    }
    return lastSym.st_value + lastSym.st_size > info.vaddr &&
            addr >= lastSym.st_value &&
            addr < lastSym.st_value + lastSym.st_size;
}

static bool pushSymNameFromVaddr(const File::DisasmCBInfo &info,
        QStringList &labels, QStringList &tooltips, Elf64_Addr addr)
{
    Elf64_Off fileOff;
    const char *symName;
    if(info.convertAddr->vaddrToFileOff(fileOff, addr) &&
            (symName = info.file->getSymNameByFileOff(fileOff)))
    {
        labels.push_back(symName);
        tooltips.push_back(getTooltip(labels.last()));
        return true;
    }
    return false;
}

static bool pushRelocNameFromVaddr(const File::DisasmCBInfo &info,
        QStringList &labels, QStringList &tooltips, Elf64_Addr addr)
{
    Elf64_Off fileOff;
    const char *relocName;
    if(info.convertAddr->vaddrToFileOff(fileOff, addr) &&
            (relocName = info.file->getRelocNameByFileOff(fileOff, fileOff+1)))
    {
        labels.push_back(relocName);
        tooltips.push_back(getTooltip(labels.last()));
        labels.last().prepend("Reloc: ");
        return true;
    }
    return false;
}

static bool pushSecOffFromVaddr(const File::DisasmCBInfo &info,
        QStringList &labels, QStringList &tooltips, Elf64_Addr addr)
{
    char *buf = info.convertAddr->vaddrToSecOffStrWithOrig(addr);
    if(buf) {
        labels.push_back(buf);
        tooltips.push_back(getTooltip(addr, info));
        free(buf);
        return true;
    }
    return false;
}

static void translateAddrToLabels(const File::DisasmCBInfo &info,
        QStringList &labels, QStringList &tooltips, Elf64_Addr addr,
        InstData &saveData)
{
    if(pushSymNameFromVaddr(info, labels, tooltips, addr)) {
        if(saveData.addrType == InstData::AT_NONE) {
            saveData.addrType = InstData::AT_SYMBOL;
            saveData.d.symbol.addr = addr;
            saveData.d.symbol.symName = strdup(labels.last().toUtf8().data());
        }
    } else if(pushRelocNameFromVaddr(info, labels, tooltips, addr)) {
        if(saveData.addrType == InstData::AT_NONE) {
            Elf64_Off fileOff;
            info.convertAddr->vaddrToFileOff(fileOff, addr);
            saveData.addrType = InstData::AT_RELOC;
            saveData.d.reloc.start = fileOff;
            saveData.d.reloc.end = fileOff+1;
            saveData.d.reloc.symName = strdup(labels.last().toUtf8().data());
        }
    } else if(pushSecOffFromVaddr(info, labels, tooltips, addr)) {
        if(saveData.addrType == InstData::AT_NONE) {
            saveData.addrType = InstData::AT_VADDR;
            saveData.d.vaddr.addr = addr;
        }
    }
}

QString LoadWorker::processBuffer(const File::DisasmInstInfo &inst,
        const File::DisasmCBInfo &info, InstData &saveData)
{
    saveData.addrType = InstData::AT_NONE;

    QStringList labels, tooltips;

    //----------Label
    if(inst.hasLabel) {
        translateAddrToLabels(info, labels, tooltips, inst.label, saveData);
    }

    //----------New label from arguments
    foreach(const QString &arg, inst.args) {
        bool ok;
        Elf64_Addr addr;
        if(arg.startsWith('$')) {
            addr = arg.right(arg.length()-1).toULong(&ok, 0);
        } else {
            addr = arg.toULong(&ok, 0);
        }
        if(!ok) continue;
        if(sameSymbol(addr, info)) {
            saveData.addrType = InstData::AT_VADDR_INSYM;
            saveData.d.insym.addr = addr;
            if(((LoadWorker*)info.data)->_ehdr.e_type == ET_REL) {
                size_t shndx;
                Elf64_Off shOff;
                info.convertAddr->fileOffToSecOff(shndx, shOff,
                        info.last - (const uint8_t*)info.file->getRawData(0));
                info.convertAddr->secOffToFileOff(
                        saveData.d.insym.off, shndx, addr);
            } else {
                info.convertAddr->vaddrToFileOff(
                        saveData.d.insym.off, addr);
            }
            continue;
        }
        if(((LoadWorker*)info.data)->_ehdr.e_type == ET_REL) {
            continue;
        }
        translateAddrToLabels(info, labels, tooltips, addr, saveData);
    }

    //----------.o relocs
    const char *relocName;
    if((relocName = info.file->getRelocNameByFileOff(
                    (const char*)info.last - info.file->getRawData(0),
                    (const char*)info.cur - info.file->getRawData(0))))
    {
        labels.push_back(relocName);
        tooltips.push_back(getTooltip(labels.last()));
        labels.last().prepend("Reloc: ");

        saveData.addrType = InstData::AT_RELOC;
        saveData.d.reloc.start =
            (const char*)info.last - info.file->getRawData(0);
        saveData.d.reloc.end =
            (const char*)info.cur - info.file->getRawData(0);
        saveData.d.reloc.symName = strdup(relocName);
    }

    //----------Compile
    QString res = inst.comm;
    if(!inst.args.isEmpty()) {
        res += "\t" + inst.args.join(",");
    }
    if(!labels.isEmpty()) {
        res += "\t# <" + labels.join("> <") + ">";
        if(!tooltips.isEmpty()) {
            res += "\x1f" + tooltips.join("\n");
        }
    }
    return res;
}

int LoadWorker::disasmCallback(const File::DisasmInstInfo &inst,
        File::DisasmCBInfo &info)
{
    LoadWorker *worker = (LoadWorker*)info.data;
    InfoModel *infoModel = worker->_infoModel;

    //-----------Raw bytes
    QString bytes;
    for(const uint8_t *p=info.last; p != info.cur; p++) {
        if(!bytes.isEmpty()) {
            bytes += " ";
        }
        bytes += QString("%1").arg(*p, 2, 16, QChar('0'));
    }

    //-----------Detect symbol start
    const char *symName = info.file->getSymNameByFileOff(
            info.last - (const uint8_t*)info.file->getRawData(0));
    if(symName) {
        char *demangled = cplus_demangle(symName);
        QModelIndex inserted = infoModel->buildMore(
                QString("\t").repeated(worker->_instIndentLevel) +
                QString("%1\x1f%2")
                .arg(symName)
                .arg(demangled));

        InstData instData;
        instData.addrType = InstData::AT_SYMBOL_START;
        instData.d.start.off = info.last - (uint8_t*)info.file->getRawData(0);
        instData.d.start.symName = strdup(symName);
        QVariant userData;
        userData.setValue(instData);
        infoModel->setData(inserted, userData, Qt::UserRole);

        worker->symbolStarted(inserted);
        free(demangled);
    }

    //-----------Compile
    InstData instData;
    QModelIndex inserted =
        infoModel->buildMore(QString("\t").repeated(worker->_instIndentLevel)+
                QString("0x%1\t%2\t%3")
                .arg(info.vaddr, 0, 16)
                .arg(bytes)
                .arg(processBuffer(inst, info, instData)));
    QVariant userData;
    userData.setValue(instData);
    infoModel->setData(inserted, userData, Qt::UserRole);
    infoModel->metadata().value<DisasmMetadata*>()->instMap[
        info.last - (const uint8_t*)info.file->getRawData(0)] = inserted;

    //-----------After work
    worker->_noSleep += info.cur - info.last;
    info.vaddr += info.cur - info.last;
    info.last = info.cur;
    if(worker->_noSleep > 0x1000) {
        msleep(100);
        worker->_noSleep = 0;
    }
    worker->_instCnt++;
    return 0;
}

void LoadWorker::stopTimer()
{
    _tm.end();
    emit timerStopped(_tm.getDiff(), _tm.getDiff()/_instCnt);
}

void LoadWorker::run()
{
    _tm.start();
    QObject::connect(this, SIGNAL(finished()),
            this, SLOT(stopTimer()));
    QObject::connect(this, SIGNAL(terminated()),
            this, SLOT(stopTimer()));
    _noSleep = 0;
    _instCnt = 0;
    if(!_file->getEhdr(&_ehdr)) {
        return;
    }
    if(_infoModel->metadata().isValid()) {
        delete _infoModel->metadata().value<DisasmMetadata*>();
    }
    _infoModel->metadata().setValue(new DisasmMetadata());
    if(_restricted) {
        _instIndentLevel = 0;
        _file->disasm(_begin, _end, disasmCallback, this);
    } else {
        _instIndentLevel = 1;
        runAll();
    }
}

void LoadWorker::runAll()
{
    size_t shdrNum;
    if(_file->getShdrNum(&shdrNum) != 0) {
        return;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!_file->getShdr(i, &shdr)) {
            continue;
        }
        if(shdr.sh_type != SHT_PROGBITS || shdr.sh_size == 0 ||
                (shdr.sh_flags & SHF_EXECINSTR) == 0)
        {
            continue;
        }
        QModelIndex ins = _infoModel->buildMore(QString("Section   %1")
                .arg(_file->getScnName(&shdr)));
        emit symbolStarted(ins);
        _file->disasm(shdr.sh_offset, shdr.sh_offset+shdr.sh_size,
                disasmCallback, this);
    }
}

END_PLUG_NAMESPACE
