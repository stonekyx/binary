/*
 * Copyright 2015 KANG Yuxuan
 *
 * ConvertAddr.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <QtCore/QString>
#include <cstdio>

#include "ConvertAddr.h"

BEGIN_BIN_NAMESPACE(backend)

ConvertAddr::ConvertAddr(File *file) :
    _file(file)
{
    if(file) {
        file->registerWatcher(this);
        QObject::connect(file, SIGNAL(aboutToBeDestroyed()),
                this, SLOT(invalidate()));
        size_t phdrNum;
        if(_file->getPhdrNum(&phdrNum) == 0) {
            for(size_t i=0; i<phdrNum; i++) {
                Elf64_Phdr phdr;
                if(!_file->getPhdr(i, &phdr)) {
                    _phdrs.clear();
                    break;
                }
                _phdrs.push_back(phdr);
            }
        } //phdr available
        size_t shdrNum;
        if(_file->getShdrNum(&shdrNum) == 0) {
            for(size_t i=0; i<shdrNum; i++) {
                Elf64_Shdr shdr;
                if(!_file->getShdr(i, &shdr)) {
                    _shdrs.clear();
                    break;
                }
                _shdrs.push_back(shdr);
            }
        } //shdr available
        size_t shdrStrNdx;
        Elf64_Shdr shdrStrShdr;
        if(_file->getShdrStrNdx(&shdrStrNdx) == 0 &&
                _file->getShdr(shdrStrNdx, &shdrStrShdr))
        {
            _shdrStrRaw = _file->getRawData(shdrStrShdr.sh_offset);
        }
    }
}

bool ConvertAddr::vaddrToFileOff(Elf64_Off &dst, Elf64_Addr addr)
{
    if(!_file) {
        return false;
    }
    for(size_t i=0; i<_phdrs.size(); i++) {
        if(addr >= _phdrs[i].p_vaddr &&
                addr < _phdrs[i].p_vaddr+_phdrs[i].p_memsz)
        {
            dst = _phdrs[i].p_offset + (addr - _phdrs[i].p_vaddr);
            return true;
        }
    }
    return false;
}

bool ConvertAddr::fileOffToVaddr(Elf64_Addr &dst, Elf64_Off offset)
{
    if(!_file) {
        return false;
    }
    for(size_t i=0; i<_phdrs.size(); i++) {
        if(offset >= _phdrs[i].p_offset &&
                offset < _phdrs[i].p_offset+_phdrs[i].p_filesz)
        {
            dst = _phdrs[i].p_vaddr + (offset - _phdrs[i].p_offset);
            return true;
        }
    }
    return false;
}

bool ConvertAddr::vaddrToSecOff(size_t &scnIdx, Elf64_Off &scnOff,
        Elf64_Addr addr)
{
    if(!_file) {
        return false;
    }
    for(size_t i=0; i<_shdrs.size(); i++) {
        if(!(_shdrs[i].sh_flags & SHF_ALLOC)) {
            continue;
        }
        if(addr >= _shdrs[i].sh_addr &&
                addr < _shdrs[i].sh_addr+_shdrs[i].sh_size)
        {
            scnIdx = i;
            scnOff = addr - _shdrs[i].sh_addr;
            return true;
        }
    }
    return false;
}

bool ConvertAddr::fileOffToSecOff(size_t &scnIdx, Elf64_Off &scnOff,
        Elf64_Off offset)
{
    if(!_file) {
        return false;
    }
    for(size_t i=0; i<_shdrs.size(); i++) {
        if(offset >= _shdrs[i].sh_offset &&
                offset < _shdrs[i].sh_offset+_shdrs[i].sh_size)
        {
            scnIdx = i;
            scnOff = offset - _shdrs[i].sh_offset;
            return true;
        }
    }
    return false;
}

bool ConvertAddr::secOffToFileOff(Elf64_Off &dst,
        size_t scnIdx, Elf64_Off scnOff)
{
    if(!_file || scnIdx >= _shdrs.size() || scnOff > _shdrs[scnIdx].sh_size) {
        return false;
    }
    dst = _shdrs[scnIdx].sh_offset + scnOff;
    return true;
}

char *ConvertAddr::secOffStr(size_t scnIdx, Elf64_Off scnOff)
{
    if(!_file || scnIdx >= _shdrs.size() || scnOff > _shdrs[scnIdx].sh_size) {
        return NULL;
    }
    QString str;
    if(_shdrStrRaw && _shdrStrRaw[_shdrs[scnIdx].sh_name]) {
        str = QString("%1+0x%2").arg(_shdrStrRaw + _shdrs[scnIdx].sh_name)
            .arg(scnOff, 0, 16);
    } else {
        str = QString("section[%1]+0x%2").arg(scnIdx).arg(scnOff, 0, 16);
    }
    return strdup(str.toUtf8().constData());
}

char *ConvertAddr::vaddrToSecOffStr(Elf64_Addr addr)
{
    size_t scnIdx;
    Elf64_Off scnOff;
    if(vaddrToSecOff(scnIdx, scnOff, addr)) {
        return secOffStr(scnIdx, scnOff);
    } else {
        return NULL;
    }
}

char *ConvertAddr::vaddrToSecOffStrWithOrig(Elf64_Addr addr)
{
    size_t scnIdx;
    Elf64_Off scnOff;
    if(vaddrToSecOff(scnIdx, scnOff, addr)) {
        char *str = secOffStr(scnIdx, scnOff);
        QString res = QString("0x%1: %2").arg(addr, 0, 16).arg(str);
        free(str);
        return strdup(res.toUtf8().constData());
    } else {
        return NULL;
    }
}

char *ConvertAddr::vaddrToSecOffStrWithData(Elf64_Addr addr, size_t ds)
{
    size_t scnIdx;
    Elf64_Off scnOff;
    if(vaddrToSecOff(scnIdx, scnOff, addr)) {
        Elf64_Shdr shdr;
        if(!_file->getShdr(scnIdx, &shdr) ||
                shdr.sh_type == SHT_NOBITS)
        {
            return secOffStr(scnIdx, scnOff);
        }
        const char *str = _file->getRawData(shdr.sh_offset);
        if(str) { str += scnOff; }
        char *buf = new char[ds+1];
        buf[0] = 0;
        for(size_t i=0; str && i<ds && str[i] && isprint(str[i]); i++) {
            buf[i] = str[i];
            buf[i+1] = 0;
        }
        char *res;
        if(buf[0]) {
            asprintf(&res, "%s: \"%s\"", secOffStr(scnIdx, scnOff), buf);
            delete[] buf;
            return res;
        } else {
            delete[] buf;
            return secOffStr(scnIdx, scnOff);
        }
    } else {
        return NULL;
    }
}

void ConvertAddr::invalidate()
{
    _file = NULL;
    emit invalidated();
}

END_BIN_NAMESPACE
