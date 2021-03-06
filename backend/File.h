/*
 * Copyright 2015 KANG Yuxuan
 *
 * File.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_FILE_H
#define BACKEND_FILE_H

#include <elf.h>
#include <cstdlib>
#include <cstring>
#include <set>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "common.h"
#include "Arhdr.h"
#include "Arsym.h"

BEGIN_BIN_NAMESPACE(backend)

class Backend;
class ConvertAddr;
class File;

END_BIN_NAMESPACE

class binary::backend::File : public QObject {
    Q_OBJECT
public:
    typedef enum {
        KIND_UNKNOWN,
        KIND_AR,
        KIND_ELF,
        KIND_COFF
    } ELFKind;
    virtual bool isValid() = 0;
    virtual int getClass() = 0;
    virtual ELFKind getKind() = 0;
    virtual bool getEhdr(Elf64_Ehdr *) = 0;
    virtual int getPhdrNum(size_t *dst) = 0;
    virtual int getShdrNum(size_t *dst) = 0;
    virtual int getShdrStrNdx(size_t *dst) = 0;
    virtual bool getPhdr(size_t, Elf64_Phdr *) = 0;
    virtual bool getShdr(size_t, Elf64_Shdr *) = 0;
    virtual const char *getScnName(Elf64_Shdr *) = 0;
    virtual ssize_t getScnData(size_t idx, void *, size_t bufsize) = 0;
    virtual bool getSym(size_t scnIdx, int idx, Elf64_Sym *) = 0;
    virtual bool getSyminfo(size_t scnIdx, int idx, Elf64_Syminfo *) = 0;
    virtual const char *getStrPtr(size_t scnIdx, size_t offset) = 0;
    virtual bool getDyn(size_t scnIdx, int idx, Elf64_Dyn *) = 0;
    virtual Arhdr getArhdr(size_t objIdx) = 0;
    virtual Arhdr getArhdrByOffset(size_t objOff) = 0;
    virtual size_t getArhdrNum() = 0;
    virtual bool setArObj(size_t objIdx) = 0;
    virtual Arsym getArsym(size_t symIdx) = 0;
    virtual size_t getArsymNum() = 0;
    virtual char *getRawData(size_t offset) = 0;
    virtual bool queryDynSym(const char *, Elf64_Sym *dst) = 0;
    virtual const char *queryDynSymDeps(const char *, Elf64_Sym *) = 0;
    struct DisasmInstInfo {
        QString comm;
        QStringList args;
        bool hasLabel;
        Elf64_Addr label;
        Elf64_Xword size;
    };
    struct DisasmCBInfo {
        const uint8_t *cur;
        const uint8_t *last;
        Elf64_Addr vaddr;
        File *file;
        void *data;
        ConvertAddr *convertAddr;
        void *callerData;
    };
    typedef int (*DisasmCB)(const DisasmInstInfo &, DisasmCBInfo &);
    virtual int disasm(Elf64_Off, Elf64_Off, DisasmCB, void*) = 0;
    virtual const char *getSymNameByFileOff(Elf64_Off) = 0;
    virtual bool getLastSymDataByFileOff(Elf64_Off, Elf64_Sym *) = 0;
    virtual const char *getRelocNameByFileOff(Elf64_Off, Elf64_Off) = 0;
    virtual bool getRel(size_t scnIdx, int idx, Elf64_Rel *) = 0;
    virtual bool getRela(size_t scnIdx, int idx, Elf64_Rela *) = 0;
    virtual char *findDynTag(Elf64_Sxword) = 0;
    virtual size_t detectDynSymCnt() = 0;
    virtual bool getSymFileOff(Elf64_Off *, const Elf64_Sym *) = 0;
    virtual const char *getName() {
        return _name;
    }

    void setBackend(Backend *b) {
        _backend = b;
    }
    void registerWatcher(const QObject *o) {
        _watchers.insert(o);
        QObject::connect(o, SIGNAL(destroyed()),
                this, SLOT(admitDestroy()));
    }
    void arrangeDelete() {
        if(_watchers.empty()) {
            delete this;
            return;
        }
        _arrangedDelete = true;
        emit aboutToBeDestroyed();
    }
signals:
    void aboutToBeDestroyed();
public slots:
    void admitDestroy() {
        _watchers.erase(sender());
        if(_arrangedDelete && _watchers.empty()) {
            deleteLater();
        }
    }
protected:
    Backend *_backend;
    char *_name;
    std::set<const QObject*> _watchers;
    bool _arrangedDelete;

    File(const char *name) :
        _backend(NULL), _name(strdup(name)), _arrangedDelete(false) {}
    virtual ~File() { free(_name); }
};

#endif
