/*
 * Copyright 2015 KANG Yuxuan
 *
 * FileImplLibelf.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef BACKEND_FILEIMPL_LIBELF_H
#define BACKEND_FILEIMPL_LIBELF_H

#include <libelf.h>
#include <vector>
#include <map>
#include <elfutils/libasm.h>

#include "common.h"
#include "Arhdr.h"
#include "Arsym.h"
#include "File.h"

BEGIN_BIN_NAMESPACE(backend)

class FileImplLibelf : public File {
public:
    FileImplLibelf(const char *, Elf_Cmd);
    virtual bool isValid();
    virtual int getClass();
    virtual ELFKind getKind();
    virtual bool getEhdr(Elf64_Ehdr *);
    virtual int getPhdrNum(size_t *);
    virtual int getShdrNum(size_t *);
    virtual int getShdrStrNdx(size_t *);
    virtual bool getPhdr(size_t, Elf64_Phdr *);
    virtual bool getShdr(size_t, Elf64_Shdr *);
    virtual const char *getScnName(Elf64_Shdr *);
    virtual ssize_t getScnData(size_t idx, void *, size_t bufsize);
    virtual bool getSym(size_t scnIdx, int idx, Elf64_Sym *);
    virtual bool getSyminfo(size_t scnIdx, int idx, Elf64_Syminfo *);
    virtual const char *getStrPtr(size_t scnIdx, size_t offset);
    virtual bool getDyn(size_t scnIdx, int idx, Elf64_Dyn *);
    virtual Arhdr getArhdr(size_t objIdx);
    virtual Arhdr getArhdrByOffset(size_t objOff);
    virtual size_t getArhdrNum();
    virtual bool setArObj(size_t objIdx);
    virtual Arsym getArsym(size_t symIdx);
    virtual size_t getArsymNum();
    virtual char *getRawData(size_t offset);
    virtual bool queryDynSym(const char *, Elf64_Sym *dst);
    virtual const char *queryDynSymDeps(const char *, Elf64_Sym *);
    virtual int disasm(Elf64_Off, Elf64_Off, DisasmCB, void*);
    virtual const char *getSymNameByFileOff(Elf64_Off);
    virtual bool getLastSymDataByFileOff(Elf64_Off, Elf64_Sym *);
    virtual const char *getRelocNameByFileOff(Elf64_Off, Elf64_Off);
    virtual bool getRel(size_t scnIdx, int idx, Elf64_Rel *);
    virtual bool getRela(size_t scnIdx, int idx, Elf64_Rela *);
    virtual char *findDynTag(Elf64_Sxword);
    virtual size_t detectDynSymCnt();
    virtual bool getSymFileOff(Elf64_Off *, const Elf64_Sym *);
    virtual ~FileImplLibelf();
private:
    int _fd;
    Elf *_elf;
    Elf *_ar;
    int _arPos;
    struct ScnDataCache {
        ScnDataCache() { reset(); }
        void reset() { data = NULL; idx = 0; }
        Elf_Data *data;
        size_t idx;
    }_symTabCache, _dynCache, _relCache, _relaCache;
    std::vector<Arhdr> _arhdr;
    std::vector<Arsym> _arsym;
    char *_hashTabRaw;
    char *_dynSymRaw;
    char *_dynStrRaw;
    std::vector<File*> _deps;
    bool _depsLoaded;
    DisasmCtx_t *_disasmCtx;
    Ebl *_ebl;
    std::map<Elf64_Off, const char *> _symNameMap;
    std::map<Elf64_Off, Elf64_Sym> _symDataMap;
    std::map<Elf64_Off, const char *> _relocNameMap;
    struct DisasmPrivData {
        std::map<Elf64_Off, const char *> *symNameMap;
        std::map<Elf64_Off, Elf64_Sym> *symDataMap;
        std::map<Elf64_Off, const char *> *relocNameMap;
        DisasmCB outputCB;
    };
    std::vector<char *> _mallocStrings;

    bool readArhdr();
    bool rewindAr();
    void readArsym();
    template<typename WordType, typename SymType>
    bool queryDynSymC(const char *name, Elf64_Sym *dst);
    bool loadDeps(const char *);
    void resetDeps();
    void resetDisasm();
    void prepareSymLookup();
    static int disasmOutput(char *, size_t, void *);
    bool checkScnData(size_t scnIdx, ScnDataCache *cache);
    void prepareRelocLookup();
    void preparePltSym();
};

END_BIN_NAMESPACE

#endif
