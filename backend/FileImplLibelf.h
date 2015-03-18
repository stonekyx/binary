#ifndef BACKEND_FILEIMPL_LIBELF_H
#define BACKEND_FILEIMPL_LIBELF_H

#include <libelf.h>

#include "common.h"
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
    virtual ~FileImplLibelf();
private:
    int _fd;
    Elf *_elf;
    Elf_Data *_symTabData;
    size_t _symTabIdx;
};

END_BIN_NAMESPACE

#endif
