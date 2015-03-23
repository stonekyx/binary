#include <cstdlib>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ar.h>
#include <libelf.h>
#include <gelf.h>

#include "File.h"
#include "Backend.h"
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
    _ar = NULL;
    _elf = elf_begin(_fd, cmd, NULL);
    if(elf_kind(_elf) == ELF_K_AR) {
        _ar = _elf;
        _arPos = -1;
        if(!readArhdr()) {
            elf_end(_ar);
            _ar = _elf = NULL;
        } else {
            readArsym();
        }
    }
    _symTabData = NULL;
    _symTabIdx = 0;
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

bool FileImplLibelf::getPhdr(size_t idx, Elf64_Phdr *dst)
{
    GElf_Phdr phdr;
    if(!gelf_getphdr(_elf, idx, &phdr)) {
        return false;
    }
    dst->p_type = phdr.p_type;
    dst->p_flags = phdr.p_flags;
    dst->p_offset = phdr.p_offset;
    dst->p_vaddr = phdr.p_vaddr;
    dst->p_paddr = phdr.p_paddr;
    dst->p_filesz = phdr.p_filesz;
    dst->p_memsz = phdr.p_memsz;
    dst->p_align = phdr.p_align;
    return true;
}

bool FileImplLibelf::getShdr(size_t idx, Elf64_Shdr *dst)
{
    Elf_Scn *scn;
    GElf_Shdr shdr;
    if(!(scn=elf_getscn(_elf, idx)) || !gelf_getshdr(scn, &shdr)) {
        return false;
    }
    dst->sh_name = shdr.sh_name;
    dst->sh_type = shdr.sh_type;
    dst->sh_flags = shdr.sh_flags;
    dst->sh_addr = shdr.sh_addr;
    dst->sh_offset = shdr.sh_offset;
    dst->sh_size = shdr.sh_size;
    dst->sh_link = shdr.sh_link;
    dst->sh_info = shdr.sh_info;
    dst->sh_addralign = shdr.sh_addralign;
    dst->sh_entsize = shdr.sh_entsize;
    return true;
}

const char *FileImplLibelf::getScnName(Elf64_Shdr *shdr)
{
    size_t shdrStrNdx;
    if(getShdrStrNdx(&shdrStrNdx) != 0) {
        return NULL;
    }
    return elf_strptr(_elf, shdrStrNdx, shdr->sh_name);
}

ssize_t FileImplLibelf::getScnData(size_t idx, void *buf, size_t bufsize)
{
    Elf_Scn *scn;
    GElf_Shdr shdr;
    if(!(scn=elf_getscn(_elf, idx)) || !gelf_getshdr(scn, &shdr)) {
        return -1;
    }
    Elf_Data *data = NULL;
    size_t n = 0;
    while(n<shdr.sh_size && n<bufsize &&
            (data = elf_getdata(scn, data)) != NULL)
    {
        if(!data->d_buf) {
            break;
        }
        size_t copySize = data->d_size;
        if(bufsize-n < copySize) {
            copySize = bufsize-n;
        }
        memcpy((char*)buf+n, data->d_buf, copySize);
        n += copySize;
    }
    return n;
}

bool FileImplLibelf::getSym(size_t scnIdx, int idx, Elf64_Sym *dst)
{
    if(scnIdx != _symTabIdx) {
        Elf_Scn *scn = elf_getscn(_elf, scnIdx);
        Elf_Data *data = NULL;
        if(!scn || (data=elf_getdata(scn, data))==NULL) {
            return false;
        }
        _symTabIdx = scnIdx;
        _symTabData = data;
    }
    GElf_Sym sym;
    gelf_getsym(_symTabData, idx, &sym);
    dst->st_name = sym.st_name;
    dst->st_info = sym.st_info;
    dst->st_other = sym.st_other;
    dst->st_shndx = sym.st_shndx;
    dst->st_value = sym.st_value;
    dst->st_size = sym.st_size;
    return true;
}

bool FileImplLibelf::getSyminfo(size_t scnIdx, int idx, Elf64_Syminfo *dst)
{
    if(scnIdx != _symTabIdx) {
        Elf_Scn *scn = elf_getscn(_elf, scnIdx);
        Elf_Data *data = NULL;
        if(!scn || (data=elf_getdata(scn, data))==NULL) {
            return false;
        }
        _symTabIdx = scnIdx;
        _symTabData = data;
    }
    GElf_Syminfo syminfo;
    gelf_getsyminfo(_symTabData, idx, &syminfo);
    dst->si_boundto = syminfo.si_boundto;
    dst->si_flags = syminfo.si_flags;
    return true;
}

const char *FileImplLibelf::getStrPtr(size_t scnIdx, size_t offset)
{
    return elf_strptr(_elf, scnIdx, offset);
}

bool FileImplLibelf::getDyn(size_t scnIdx, int idx, Elf64_Dyn *dst)
{
    if(scnIdx != _dynIdx) {
        Elf_Scn *scn = elf_getscn(_elf, scnIdx);
        Elf_Data *data = NULL;
        if(!scn || (data=elf_getdata(scn, data))==NULL) {
            return false;
        }
        _dynIdx = scnIdx;
        _dynData = data;
    }
    GElf_Dyn dyn;
    gelf_getdyn(_dynData, idx, &dyn);
    dst->d_tag = dyn.d_tag;
    dst->d_un = dyn.d_un;
    return true;
}

Arhdr FileImplLibelf::getArhdr(size_t objIdx)
{
    if(objIdx>=_arhdr.size()) {
        return Arhdr();
    }
    return _arhdr[objIdx];
}

Arhdr FileImplLibelf::getArhdrByOffset(size_t objOff)
{
    if(elf_rand(_ar, objOff) == 0) {
        return Arhdr();
    }
    Elf *obj = elf_begin(_fd, ELF_C_READ, _ar);
    if(!obj) {
        return Arhdr();
    }
    Arhdr res(elf_getarhdr(obj), objOff);
    elf_end(obj);
    return res;
}

size_t FileImplLibelf::getArhdrNum()
{
    return _arhdr.size();
}

bool FileImplLibelf::setArObj(size_t objIdx)
{
    if(objIdx >= _arhdr.size()) {
        return false;
    }
    if(elf_rand(_ar, _arhdr[objIdx].offset()) == 0) {
        return false;
    }
    _elf = elf_begin(_fd, ELF_C_READ, _ar);
    _backend->signalFileChange(NULL);
    _backend->signalFileChange(this);
    return _elf;
}

Arsym FileImplLibelf::getArsym(size_t symIdx)
{
    if(symIdx>=_arsym.size()) {
        return Arsym();
    }
    return _arsym[symIdx];
}

size_t FileImplLibelf::getArsymNum()
{
    return _arsym.size();
}

FileImplLibelf::~FileImplLibelf()
{
    if(_elf) {
        elf_end(_elf);
    }
    if(_ar && _arPos>=0) {
        elf_end(_ar);
    }
    close(_fd);
}

bool FileImplLibelf::readArhdr()
{
    Elf *e;
    while((e = elf_begin(_fd, ELF_C_READ, _ar))!=NULL) {
        Elf_Arhdr *arh;
        if((arh = elf_getarhdr(e)) == NULL) {
            elf_end(e);
            return false;
        }
        _arhdr.push_back(Arhdr(arh, elf_getaroff(e)));
        elf_next(e);
        elf_end(e);
    }
    return rewindAr();
}

bool FileImplLibelf::rewindAr()
{
    if(elf_rand(_ar, (size_t)SARMAG) == SARMAG) {
        return true;
    }
    return false;
}

void FileImplLibelf::readArsym()
{
    size_t arsymNum;
    Elf_Arsym *arsym = elf_getarsym(_ar, &arsymNum);
    for(size_t i=0; i<arsymNum; i++) {
        _arsym.push_back(Arsym(arsym+i));
    }
}

END_BIN_NAMESPACE
