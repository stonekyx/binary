#include "ConvertAddr.h"

BEGIN_BIN_NAMESPACE(backend)

ConvertAddr::ConvertAddr(File *file) :
    _file(file)
{
    if(file) {
        file->registerWatcher(this);
        QObject::connect(file, SIGNAL(aboutToBeDestroyed()),
                this, SLOT(invalidate()));
    }
}

bool ConvertAddr::vaddrToFileOff(Elf64_Off &dst, Elf64_Addr addr)
{
    if(!_file) {
        return false;
    }
    size_t phdrNum;
    if(_file->getPhdrNum(&phdrNum) != 0) {
        return false;
    }
    for(size_t i=0; i<phdrNum; i++) {
        Elf64_Phdr phdr;
        if(!_file->getPhdr(i, &phdr)) {
            continue;
        }
        if(addr >= phdr.p_vaddr && addr < phdr.p_vaddr+phdr.p_memsz) {
            dst = phdr.p_offset + (addr - phdr.p_vaddr);
            return true;
        }
    }
    return false;
}

bool ConvertAddr::fileOffToVaddr(Elf64_Addr &dst, Elf64_Off offset)
{
    if(!_file) {
        return false;
    }
    size_t phdrNum;
    if(_file->getPhdrNum(&phdrNum) != 0) {
        return false;
    }
    for(size_t i=0; i<phdrNum; i++) {
        Elf64_Phdr phdr;
        if(!_file->getPhdr(i, &phdr)) {
            continue;
        }
        if(offset >= phdr.p_offset &&
                offset < phdr.p_offset+phdr.p_filesz)
        {
            dst = phdr.p_vaddr + (offset - phdr.p_offset);
            return true;
        }
    }
    return false;
}

bool ConvertAddr::vaddrToSecOff(size_t &scnIdx, Elf64_Off &scnOff,
        Elf64_Addr addr)
{
    size_t shdrNum;
    if(!_file || _file->getShdrNum(&shdrNum) != 0) {
        return false;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!_file->getShdr(i, &shdr)) {
            continue;
        }
        if(addr >= shdr.sh_addr && addr < shdr.sh_addr+shdr.sh_size) {
            scnIdx = i;
            scnOff = addr - shdr.sh_addr;
            return true;
        }
    }
    return false;
}

bool ConvertAddr::fileOffToSecOff(size_t &scnIdx, Elf64_Off &scnOff,
        Elf64_Off offset)
{
    size_t shdrNum;
    if(!_file || _file->getShdrNum(&shdrNum) != 0) {
        return false;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!_file->getShdr(i, &shdr)) {
            continue;
        }
        if(offset >= shdr.sh_offset && offset < shdr.sh_offset+shdr.sh_size)
        {
            scnIdx = i;
            scnOff = offset - shdr.sh_offset;
            return true;
        }
    }
    return false;
}

bool ConvertAddr::secOffToFileOff(Elf64_Off &dst,
        size_t scnIdx, Elf64_Off scnOff)
{
    Elf64_Shdr shdr;
    if(!_file || !_file->getShdr(scnIdx, &shdr)) {
        return false;
    }
    dst = shdr.sh_offset + scnOff;
    return true;
}

void ConvertAddr::invalidate()
{
    _file = NULL;
    emit invalidated();
}

END_BIN_NAMESPACE
