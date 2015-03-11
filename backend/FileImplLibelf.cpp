#include <cstdlib>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <libelf.h>
#include <gelf.h>

#include "File.h"
#include "FileImplLibelf.h"

BEGIN_BIN_NAMESPACE(backend)

FileImplLibelf::FileImplLibelf(const char *name,
        Elf_Cmd cmd)
{
    static bool needInit = true;
    if(needInit) {
        needInit = false;
        if(elf_version(EV_CURRENT) == EV_NONE) {
            throw std::exception();
        }
    }
    int flags = 0;
    if(ELF_C_READ == cmd) {
        flags = O_RDONLY;
    } else if(ELF_C_WRITE == cmd) {
        flags = O_WRONLY;
    } else if(ELF_C_RDWR == cmd) {
        flags = O_RDWR;
    } else { // error
        flags = O_RDONLY;
    }
    _fd = open(name, flags);
    _elf = elf_begin(_fd, cmd, NULL);
}

int FileImplLibelf::getClass()
{
    return gelf_getclass(_elf);
}

FileImplLibelf::~FileImplLibelf()
{
    elf_end(_elf);
    close(_fd);
}

END_BIN_NAMESPACE
