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

bool FileImplLibelf::isValid()
{
    return _elf;
}

int FileImplLibelf::getClass()
{
    return gelf_getclass(_elf);
}

File::ELFKind FileImplLibelf::getKind()
{
    Elf_Kind kind = elf_kind(_elf);
    switch(kind) {
    case ELF_K_AR:
        return KIND_AR;
    case ELF_K_ELF:
        return KIND_ELF;
    case ELF_K_COFF:
        return KIND_COFF;
    default:
        return KIND_UNKNOWN;
    }
}

bool FileImplLibelf::getEhdr(Elf64_Ehdr *dst)
{
    GElf_Ehdr ehdr;
    if(!gelf_getehdr(_elf, &ehdr)) {
        return false;
    }
    memcpy(&dst->e_ident, &ehdr.e_ident, sizeof(ehdr.e_ident));
    dst->e_type = ehdr.e_type;
    dst->e_machine = ehdr.e_machine;
    dst->e_version = ehdr.e_version;
    dst->e_entry = ehdr.e_entry;
    dst->e_phoff = ehdr.e_phoff;
    dst->e_shoff = ehdr.e_shoff;
    dst->e_flags = ehdr.e_flags;
    dst->e_ehsize = ehdr.e_ehsize;
    dst->e_phentsize = ehdr.e_phentsize;
    dst->e_phnum = ehdr.e_phnum;
    dst->e_shentsize = ehdr.e_shentsize;
    dst->e_shnum = ehdr.e_shnum;
    dst->e_shstrndx = ehdr.e_shstrndx;
    return true;
}

int FileImplLibelf::getPhdrNum(size_t *dst)
{
    return elf_getphdrnum(_elf, dst);
}

int FileImplLibelf::getShdrNum(size_t *dst)
{
    return elf_getshdrnum(_elf, dst);
}

int FileImplLibelf::getShdrStrNdx(size_t *dst)
{
    return elf_getshdrstrndx(_elf, dst);
}

FileImplLibelf::~FileImplLibelf()
{
    elf_end(_elf);
    close(_fd);
}

END_BIN_NAMESPACE
