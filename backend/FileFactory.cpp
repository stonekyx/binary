#include <cstdlib>

#include "FileImplLibelf.h"

#include "FileFactory.h"

BEGIN_BIN_NAMESPACE(backend)

File *FileFactory::openFile(const char *name)
{
    if(BACKEND_LIBELF == _backend) {
        return new FileImplLibelf(name, ELF_C_READ);
    } else {
        return NULL;
    }
}

END_BIN_NAMESPACE
