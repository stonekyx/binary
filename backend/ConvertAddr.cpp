#include <cstdio>

#include "ConvertAddr.h"

BEGIN_BIN_NAMESPACE(backend)

ConvertAddr::ConvertAddr(File *file) :
    _file(file)
{
    if(file) {
        file->registerWatcher(this);
        QObject::connect(file, SIGNAL(aboutToBeDestroyed()),
                this, SLOT(invalidate()));
        size_t phdrNum;
        if(_file->getPhdrNum(&phdrNum) == 0) {
            for(size_t i=0; i<phdrNum; i++) {
                Elf64_Phdr phdr;
                if(!_file->getPhdr(i, &phdr)) {
                    _phdrs.clear();
                    break;
                }
                _phdrs.push_back(phdr);
            }
        } //phdr available
        size_t shdrNum;
        if(_file->getShdrNum(&shdrNum) == 0) {
            for(size_t i=0; i<shdrNum; i++) {
                Elf64_Shdr shdr;
                if(!_file->getShdr(i, &shdr)) {
                    _shdrs.clear();
                    break;
                }
                _shdrs.push_back(shdr);
            }
        } //shdr available
        size_t shdrStrNdx;
        Elf64_Shdr shdrStrShdr;
        if(_file->getShdrStrNdx(&shdrStrNdx) == 0 &&
                _file->getShdr(shdrStrNdx, &shdrStrShdr))
        {
            _shdrStrRaw = _file->getRawData(shdrStrShdr.sh_offset);
        }
    }
}

bool ConvertAddr::vaddrToFileOff(Elf64_Off &dst, Elf64_Addr addr)
{
    if(!_file) {
        return false;
    }
    for(size_t i=0; i<_phdrs.size(); i++) {
        if(addr >= _phdrs[i].p_vaddr &&
                addr < _phdrs[i].p_vaddr+_phdrs[i].p_memsz)
        {
            dst = _phdrs[i].p_offset + (addr - _phdrs[i].p_vaddr);
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
    for(size_t i=0; i<_phdrs.size(); i++) {
        if(offset >= _phdrs[i].p_offset &&
                offset < _phdrs[i].p_offset+_phdrs[i].p_filesz)
        {
            dst = _phdrs[i].p_vaddr + (offset - _phdrs[i].p_offset);
            return true;
        }
    }
    return false;
}

bool ConvertAddr::vaddrToSecOff(size_t &scnIdx, Elf64_Off &scnOff,
        Elf64_Addr addr)
{
    if(!_file) {
        return false;
    }
    for(size_t i=0; i<_shdrs.size(); i++) {
        if(!(_shdrs[i].sh_flags & SHF_ALLOC)) {
            continue;
        }
        if(addr >= _shdrs[i].sh_addr &&
                addr < _shdrs[i].sh_addr+_shdrs[i].sh_size)
        {
            scnIdx = i;
            scnOff = addr - _shdrs[i].sh_addr;
            return true;
        }
    }
    return false;
}

bool ConvertAddr::fileOffToSecOff(size_t &scnIdx, Elf64_Off &scnOff,
        Elf64_Off offset)
{
    if(!_file) {
        return false;
    }
    for(size_t i=0; i<_shdrs.size(); i++) {
        if(offset >= _shdrs[i].sh_offset &&
                offset < _shdrs[i].sh_offset+_shdrs[i].sh_size)
        {
            scnIdx = i;
            scnOff = offset - _shdrs[i].sh_offset;
            return true;
        }
    }
    return false;
}

bool ConvertAddr::secOffToFileOff(Elf64_Off &dst,
        size_t scnIdx, Elf64_Off scnOff)
{
    if(!_file || scnIdx >= _shdrs.size() || scnOff >= _shdrs[scnIdx].sh_size) {
        return false;
    }
    dst = _shdrs[scnIdx].sh_offset + scnOff;
    return true;
}

char *ConvertAddr::secOffStr(size_t scnIdx, Elf64_Off scnOff)
{
    char *res;
    if(!_file || scnIdx >= _shdrs.size() || scnOff >= _shdrs[scnIdx].sh_size) {
        return NULL;
    }
    if(_shdrStrRaw && _shdrStrRaw[_shdrs[scnIdx].sh_name]) {
        asprintf(&res, "%s+0x%lx",
                _shdrStrRaw + _shdrs[scnIdx].sh_name, scnOff);
    } else {
        asprintf(&res, "section[%lu]+0x%lx", scnIdx, scnOff);
    }
    return res;
}

char *ConvertAddr::vaddrToSecOffStr(Elf64_Addr addr)
{
    size_t scnIdx;
    Elf64_Off scnOff;
    if(vaddrToSecOff(scnIdx, scnOff, addr)) {
        return secOffStr(scnIdx, scnOff);
    } else {
        return NULL;
    }
}

char *ConvertAddr::vaddrToSecOffStrWithOrig(Elf64_Addr addr)
{
    size_t scnIdx;
    Elf64_Off scnOff;
    if(vaddrToSecOff(scnIdx, scnOff, addr)) {
        char *str = secOffStr(scnIdx, scnOff);
        char *res;
        asprintf(&res, "%#lx: %s", addr, str);
        free(str);
        return res;
    } else {
        return NULL;
    }
}

char *ConvertAddr::vaddrToSecOffStrWithData(Elf64_Addr addr, size_t ds)
{
    size_t scnIdx;
    Elf64_Off scnOff;
    if(vaddrToSecOff(scnIdx, scnOff, addr)) {
        Elf64_Shdr shdr;
        if(!_file->getShdr(scnIdx, &shdr) ||
                shdr.sh_type == SHT_NOBITS)
        {
            return secOffStr(scnIdx, scnOff);
        }
        const char *str = _file->getRawData(shdr.sh_offset);
        if(str) { str += scnOff; }
        char *buf = new char[ds+1];
        buf[0] = 0;
        for(size_t i=0; str && i<ds && str[i] && isprint(str[i]); i++) {
            buf[i] = str[i];
            buf[i+1] = 0;
        }
        char *res;
        if(buf[0]) {
            asprintf(&res, "%s: \"%s\"", secOffStr(scnIdx, scnOff), buf);
            delete[] buf;
            return res;
        } else {
            delete[] buf;
            return secOffStr(scnIdx, scnOff);
        }
    } else {
        return NULL;
    }
}

void ConvertAddr::invalidate()
{
    _file = NULL;
    emit invalidated();
}

END_BIN_NAMESPACE
