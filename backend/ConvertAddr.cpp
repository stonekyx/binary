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

bool ConvertAddr::vaddrToFileOff(size_t &dst, Elf64_Addr addr)
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

bool ConvertAddr::fileOffToVaddr(Elf64_Addr &dst, size_t offset)
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

void ConvertAddr::invalidate()
{
    _file = NULL;
    emit invalidated();
}

END_BIN_NAMESPACE
