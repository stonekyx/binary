#ifndef BACKEND_FILEIMPL_LIBELF_H
#define BACKEND_FILEIMPL_LIBELF_H

#include <libelf.h>

#include "common.h"
#include "File.h"

BEGIN_BIN_NAMESPACE(backend)

class FileImplLibelf : public File {
public:
    FileImplLibelf(const char *, Elf_Cmd);
    virtual int getClass();
    virtual ~FileImplLibelf();
private:
    int _fd;
    Elf *_elf;
};

END_BIN_NAMESPACE

#endif
