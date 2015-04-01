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
    virtual int disasm(size_t scnIdx, DisasmCB cb, void *cbData);
    virtual const char *getSymNameByVal(Elf64_Addr);
    virtual ~FileImplLibelf();
private:
    int _fd;
    Elf *_elf;
    Elf *_ar;
    int _arPos;
    Elf_Data *_symTabData;
    size_t _symTabIdx;
    Elf_Data *_dynData;
    size_t _dynIdx;
    std::vector<Arhdr> _arhdr;
    std::vector<Arsym> _arsym;
    char *_hashTabRaw;
    char *_dynSymRaw;
    char *_dynStrRaw;
    std::vector<File*> _deps;
    bool _depsLoaded;
    DisasmCtx_t *_disasmCtx;
    Ebl *_ebl;
    std::map<Elf64_Addr, const char *> _symNameMap;
    std::map<Elf64_Addr, Elf64_Sym> _symDataMap;
    struct DisasmPrivData {
        std::map<Elf64_Addr, const char *> *symNameMap;
        std::map<Elf64_Addr, Elf64_Sym> *symDataMap;
        DisasmCB outputCB;
    };

    bool readArhdr();
    bool rewindAr();
    void readArsym();
    char *findDynTag(Elf64_Sxword);
    template<typename WordType, typename SymType>
    bool queryDynSymC(const char *name, Elf64_Sym *dst);
    bool loadDeps(const char *);
    void resetDeps();
    void resetDisasm();
    static int disasmGetSym(GElf_Addr, Elf32_Word, GElf_Addr, char **,
            size_t *, void *);
    void prepareSymLookup();
    size_t detectDynSymCnt();
    static int disasmOutput(char *, size_t, void *);
};

END_BIN_NAMESPACE

#endif
