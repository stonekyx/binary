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

class ConvertAddr;
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
    virtual bool getRel(size_t scnIdx, int idx, Elf64_Rel *);
    virtual bool getRela(size_t scnIdx, int idx, Elf64_Rela *);
    virtual char *findDynTag(Elf64_Sxword);
    virtual size_t detectDynSymCnt();
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
    struct DisasmPrivData {
        std::map<Elf64_Off, const char *> *symNameMap;
        std::map<Elf64_Off, Elf64_Sym> *symDataMap;
        DisasmCB outputCB;
        ConvertAddr *convertAddr;
    };

    bool readArhdr();
    bool rewindAr();
    void readArsym();
    template<typename WordType, typename SymType>
    bool queryDynSymC(const char *name, Elf64_Sym *dst);
    bool loadDeps(const char *);
    void resetDeps();
    void resetDisasm();
    static int disasmGetSym(GElf_Addr, Elf32_Word, GElf_Addr, char **,
            size_t *, void *);
    void prepareSymLookup();
    static int disasmOutput(char *, size_t, void *);
    bool checkScnData(size_t scnIdx, ScnDataCache *cache);
};

END_BIN_NAMESPACE

#endif
