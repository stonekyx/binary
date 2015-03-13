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
    virtual bool getPhdr(int, Elf64_Phdr *);
    virtual ~FileImplLibelf();
private:
    int _fd;
    Elf *_elf;
};

END_BIN_NAMESPACE

#endif
