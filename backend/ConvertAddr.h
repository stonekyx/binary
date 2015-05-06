/*
 * Copyright 2015 KANG Yuxuan
 *
 * ConvertAddr.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_CONVERT_ADDR_H
#define BACKEND_CONVERT_ADDR_H

#include <vector>

#include <QtCore/QObject>

#include "common.h"
#include "File.h"

BEGIN_BIN_NAMESPACE(backend)

class ConvertAddr;

END_BIN_NAMESPACE

class binary::backend::ConvertAddr : public QObject {
    Q_OBJECT
public:
    ConvertAddr(File *file);
    bool vaddrToFileOff(Elf64_Off &dst, Elf64_Addr);
    bool fileOffToVaddr(Elf64_Addr &dst, Elf64_Off);
    bool vaddrToSecOff(size_t &scnIdx, Elf64_Off &scnOff, Elf64_Addr);
    bool fileOffToSecOff(size_t &scnIdx, Elf64_Off &scnOff, Elf64_Off);
    bool secOffToFileOff(Elf64_Off &dst, size_t scnIdx, Elf64_Off scnOff);
    char *secOffStr(size_t scnIdx, Elf64_Off scnOff);
    char *vaddrToSecOffStr(Elf64_Addr);
    char *vaddrToSecOffStrWithOrig(Elf64_Addr);
    char *vaddrToSecOffStrWithData(Elf64_Addr, size_t dataSize = 20);
public slots:
    void invalidate();
signals:
    void invalidated();
private:
    File *_file;
    std::vector<Elf64_Phdr> _phdrs;
    std::vector<Elf64_Shdr> _shdrs;
    const char *_shdrStrRaw;
};

#endif
