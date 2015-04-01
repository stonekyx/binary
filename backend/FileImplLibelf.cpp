#include <iostream>
#include <cstdlib>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ar.h>
#include <libelf.h>
#include <gelf.h>
#include <elfutils/libebl.h>

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "File.h"
#include "Backend.h"
#include "ForkPipe.h"
#include "FileImplLibelf.h"

using namespace std;
BEGIN_BIN_NAMESPACE(backend)

FileImplLibelf::FileImplLibelf(const char *name,
        Elf_Cmd cmd) :
    File(name)
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
    _dynData = NULL;
    _dynIdx = 0;
    _hashTabRaw = NULL;
    _dynSymRaw = NULL;
    _dynStrRaw = NULL;
    _depsLoaded = false;
    _disasmCtx = NULL;
    _ebl = NULL;
    prepareSymLookup();
}

bool FileImplLibelf::isValid()
{
    return elf_kind(_elf) != ELF_K_NONE;
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

template<typename T>
static void convertClass(Elf64_Sym &dst, T &src)
{
    dst.st_name = src.st_name;
    dst.st_info = src.st_info;
    dst.st_other = src.st_other;
    dst.st_shndx = src.st_shndx;
    dst.st_value = src.st_value;
    dst.st_size = src.st_size;
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
    convertClass<GElf_Sym>(*dst, sym);
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
    if(_arPos != -1) {
        elf_end(_elf);
    }
    _elf = elf_begin(_fd, ELF_C_READ, _ar);
    _arPos = objIdx;
    _symTabData = NULL;
    _symTabIdx = 0;
    _dynData = NULL;
    _dynIdx = 0;
    _hashTabRaw = NULL;
    _dynSymRaw = NULL;
    _dynStrRaw = NULL;
    resetDeps();
    resetDisasm();
    prepareSymLookup();
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

char *FileImplLibelf::getRawData(size_t offset)
{
    size_t size;
    char *fileContent = elf_rawfile(_elf, &size);
    if(offset >= size) {
        return NULL;
    }
    return fileContent + offset;
}

bool FileImplLibelf::queryDynSym(const char *name, Elf64_Sym *dst)
{
    if(!_hashTabRaw && !(_hashTabRaw=findDynTag(DT_GNU_HASH))) {
        return false;
    }
    if(!_dynSymRaw && !(_dynSymRaw=findDynTag(DT_SYMTAB))) {
        return false;
    }
    if(!_dynStrRaw && !(_dynStrRaw=findDynTag(DT_STRTAB))) {
        return false;
    }
    switch(getKind()) {
    case ELFCLASS32:
        return queryDynSymC<uint32_t, Elf32_Sym>(name, dst);
    case ELFCLASS64:
        return queryDynSymC<uint64_t, Elf64_Sym>(name, dst);
    default:
        return false;
    }
}

const char *FileImplLibelf::queryDynSymDeps(const char *name,
        Elf64_Sym *dst)
{
    if(!_depsLoaded) {
        loadDeps(getName());
    }
    for(vector<File*>::iterator it = _deps.begin();
            it != _deps.end(); it++)
    {
        if((*it)->queryDynSym(name, dst)) {
            return (*it)->getName();
        }
    }
    return NULL;
}

int FileImplLibelf::disasm(size_t scnIdx, DisasmCB cb, void *cbData)
{
    if(!_disasmCtx) {
        _ebl = ebl_openbackend(_elf);
        if(!_ebl) {
            return -1;
        }
        _disasmCtx = disasm_begin(_ebl, _elf, disasmGetSym);
        if(!_disasmCtx) {
            return -1;
        }
    }
    Elf64_Shdr shdr;
    if(!getShdr(scnIdx, &shdr)) {
        return -1;
    }
    DisasmCBInfo cbInfo;
    cbInfo.cur = (const uint8_t *)getRawData(shdr.sh_offset);
    cbInfo.last = cbInfo.cur;
    cbInfo.vaddr = shdr.sh_addr;
    cbInfo.shdr = &shdr;
    cbInfo.data = cbData;
    cbInfo.symNameData = &_symNameMap;
    cbInfo.labelBuf = NULL;
    const char *fmt = "%m\t%.1o,%.2o,%.3o\t%l";
    return disasm_cb(_disasmCtx, &cbInfo.cur, cbInfo.cur+shdr.sh_size,
            shdr.sh_addr, fmt, cb, &cbInfo, &cbInfo);
}

FileImplLibelf::~FileImplLibelf()
{
    resetDisasm();
    resetDeps();
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

char *FileImplLibelf::findDynTag(Elf64_Sxword val)
{
    Elf64_Phdr phdr;
    size_t phdrNum;
    if(getPhdrNum(&phdrNum) != 0) {
        return NULL;
    }
    for(size_t i=0; i<phdrNum; i++) {
        if(!getPhdr(i, &phdr)) {
            continue;
        }
        if(phdr.p_type != PT_DYNAMIC) {
            continue;
        }
        char *dynRaw = getRawData(phdr.p_offset);
        if(getClass() == ELFCLASS32) {
            Elf32_Dyn *dyn = (Elf32_Dyn*)dynRaw;
            size_t dynNum = phdr.p_filesz / sizeof(*dyn);
            for(size_t j=0; j<dynNum; j++) {
                if(dyn[j].d_tag == val) {
                    return getRawData(dyn[j].d_un.d_ptr);
                }
            }
        } else if(getClass() == ELFCLASS64) {
            Elf64_Dyn *dyn = (Elf64_Dyn*)dynRaw;
            size_t dynNum = phdr.p_filesz / sizeof(*dyn);
            for(size_t j=0; j<dynNum; j++) {
                if(dyn[j].d_tag == val) {
                    return getRawData(dyn[j].d_un.d_ptr);
                }
            }
        } else {
            return NULL;
        }
    }
    return NULL;
}

template<typename WordType, typename SymType>
bool FileImplLibelf::queryDynSymC(const char *name, Elf64_Sym *dst)
{
    uint32_t nbuckets = *(uint32_t*)_hashTabRaw;
    uint32_t symndx = *((uint32_t*)_hashTabRaw+1);
    uint32_t maskwords = *((uint32_t*)_hashTabRaw+2);
//    uint32_t shift2 = *((uint32_t*)_hashTabRaw+3);
//    uint32_t maskwords_bm = maskwords-1;
    WordType *bloom = (WordType*)(_hashTabRaw+sizeof(uint32_t)*4);
    uint32_t *buckets = (uint32_t*)(bloom+maskwords);
    uint32_t *hashvals = (uint32_t*)(buckets+nbuckets);
//    size_t c = sizeof(WordType)*8;
    unsigned long int h1, h2;
    unsigned long int n;
//    unsigned long int bitmask;
    uint32_t *hv;
    const SymType *sym;

    h1 = elf_gnu_hash(name);
/*    h2 = h1 >> shift2;
    n = (h1/c) & maskwords_bm;
    bitmask = (1 << (h1 % c)) | (1 << (h2 % c));
    if((bloom[n] & bitmask) != bitmask) {
        return false;
    }*/

    n = buckets[h1 % nbuckets];
    if(n==0) {
        return false;
    }
    sym = ((SymType*)_dynSymRaw)+n;
    hv = &hashvals[n-symndx];
    for(h1 &= ~1; ; sym++) {
        h2 = *hv++;
        if((h1 == (h2 & ~1)) && !strcmp(name, _dynStrRaw+sym->st_name)) {
            if(dst) {
                convertClass(*dst, *sym);
            }
            return true;
        }
        if(h2 & 1) {
            break;
        }
    }
    return false;
}

bool FileImplLibelf::loadDeps(const char *name)
{
    _depsLoaded = true;
    _deps.clear();
    _deps.push_back(this);
    const char *argv[] = {"ldd", name, NULL};
    ForkPipe *forkPipe = new ForkPipe("ldd", argv);
    if(forkPipe->execAndWait() != 0) {
        return false;
    }
    QStringList lines =
        QString(forkPipe->getBuf()).split("\n", QString::SkipEmptyParts);
    foreach(QString s, lines) {
        if(!s.contains("=>")) {
            continue;
        }
        s.replace(QRegExp(".*=> (.*) \\(.*"), "\\1");
        _deps.push_back(_backend->openFilePrivate(s.toUtf8().constData()));
    }
    delete forkPipe;
    return true;
}

void FileImplLibelf::resetDeps()
{
    for(vector<File*>::iterator it = _deps.begin();
            it != _deps.end(); it++)
    {
        if(*it == this) {
            continue;
        }
        _backend->closeFilePrivate(&*it);
    }
    _depsLoaded = false;
    _deps.clear();
}

void FileImplLibelf::resetDisasm()
{
    if(!_disasmCtx) {
        return;
    }
    disasm_end(_disasmCtx);
    ebl_closebackend(_ebl);
    _disasmCtx = NULL;
    _ebl = NULL;
}

int FileImplLibelf::disasmGetSym(GElf_Addr, Elf32_Word,
        GElf_Addr val, char **buf, size_t *size, void *arg)
{
    DisasmCBInfo *info = (DisasmCBInfo *)arg;
    if(info->labelBuf) {
        free(info->labelBuf);
        info->labelBuf = NULL;
    }
    map<Elf64_Addr, const char *> *symNameMap =
        (map<Elf64_Addr, const char *> *)info->symNameData;
    if(symNameMap->find(val) != symNameMap->end()) {
        info->labelBuf = strdup((*symNameMap)[val]);
        *size = strlen(info->labelBuf);
    }
    *buf = info->labelBuf;
    return 0;
}

void FileImplLibelf::prepareSymLookup()
{
    _symNameMap.clear();
    size_t shdrNum;
    if(getShdrNum(&shdrNum)!=0) {
        return;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!getShdr(i, &shdr) || shdr.sh_type != SHT_SYMTAB) {
            continue;
        }
        for(size_t symIdx = 0;
                symIdx < shdr.sh_size/shdr.sh_entsize; symIdx++)
        {
            Elf64_Sym sym;
            if(!getSym(i, symIdx, &sym)) {
                continue;
            }
            if(getStrPtr(shdr.sh_link, sym.st_name)[0] == 0) {
                continue;
            }
            if(_symNameMap.find(sym.st_value) != _symNameMap.end()) {
                continue;
            }
            _symNameMap[sym.st_value] =
                getStrPtr(shdr.sh_link, sym.st_name);
        }
    }
}

END_BIN_NAMESPACE
