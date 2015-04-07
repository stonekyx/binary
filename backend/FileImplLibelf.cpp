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
#include "ConvertClass.h"
#include "ConvertAddr.h"
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
    _hashTabRaw = NULL;
    _dynSymRaw = NULL;
    _dynStrRaw = NULL;
    _depsLoaded = false;
    _disasmCtx = NULL;
    _ebl = NULL;
    prepareSymLookup();
    prepareRelocLookup();
}

bool FileImplLibelf::isValid()
{
    return elf_kind(_elf) != ELF_K_NONE &&
        (gelf_getclass(_elf) == ELFCLASS32 ||
         gelf_getclass(_elf) == ELFCLASS64 ||
         elf_kind(_elf) == ELF_K_AR);
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
    ConvertClass::cvt(*dst, ehdr);
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
    ConvertClass::cvt(*dst, phdr);
    return true;
}

bool FileImplLibelf::getShdr(size_t idx, Elf64_Shdr *dst)
{
    Elf_Scn *scn;
    GElf_Shdr shdr;
    if(!(scn=elf_getscn(_elf, idx)) || !gelf_getshdr(scn, &shdr)) {
        return false;
    }
    ConvertClass::cvt(*dst, shdr);
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
    if(!checkScnData(scnIdx, &_symTabCache)) {
        return false;
    }
    GElf_Sym sym;
    gelf_getsym(_symTabCache.data, idx, &sym);
    ConvertClass::cvt(*dst, sym);
    return true;
}

bool FileImplLibelf::getSyminfo(size_t scnIdx, int idx, Elf64_Syminfo *dst)
{
    if(!checkScnData(scnIdx, &_symTabCache)) {
        return false;
    }
    GElf_Syminfo syminfo;
    gelf_getsyminfo(_symTabCache.data, idx, &syminfo);
    ConvertClass::cvt(*dst, syminfo);
    return true;
}

const char *FileImplLibelf::getStrPtr(size_t scnIdx, size_t offset)
{
    return elf_strptr(_elf, scnIdx, offset);
}

bool FileImplLibelf::getDyn(size_t scnIdx, int idx, Elf64_Dyn *dst)
{
    if(!checkScnData(scnIdx, &_dynCache)) {
        return false;
    }
    GElf_Dyn dyn;
    gelf_getdyn(_dynCache.data, idx, &dyn);
    ConvertClass::cvt(*dst, dyn);
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
    _symTabCache.reset();
    _dynCache.reset();
    _relCache.reset();
    _relaCache.reset();
    _hashTabRaw = NULL;
    _dynSymRaw = NULL;
    _dynStrRaw = NULL;
    resetDeps();
    resetDisasm();
    prepareSymLookup();
    prepareRelocLookup();
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
    switch(getClass()) {
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

int FileImplLibelf::disasm(Elf64_Off begin, Elf64_Off end,
        DisasmCB cb, void *cbData)
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
    ConvertAddr convertAddr(this);
    DisasmCBInfo cbInfo;
    DisasmPrivData callerData;
    cbInfo.cur = (const uint8_t *)getRawData(begin);
    cbInfo.last = cbInfo.cur;
    if(!convertAddr.fileOffToVaddr(cbInfo.vaddr, begin)) {
        cbInfo.vaddr = begin;
    }
    cbInfo.file = this;
    cbInfo.data = cbData;
    cbInfo.labelBuf = NULL;
    cbInfo.convertAddr = &convertAddr;
    cbInfo.callerData = &callerData;
    callerData.symNameMap = &_symNameMap;
    callerData.symDataMap = &_symDataMap;
    callerData.relocNameMap = &_relocNameMap;
    callerData.outputCB = cb;
    const char *fmt = "%m\t%.1o,%.2o,%.3o\t%l";
    return disasm_cb(_disasmCtx, &cbInfo.cur, cbInfo.cur+(end-begin),
            cbInfo.vaddr, fmt, disasmOutput, &cbInfo, &cbInfo);
}

const char *FileImplLibelf::getSymNameByFileOff(Elf64_Off offset)
{
    if(_symNameMap.find(offset) != _symNameMap.end()) {
        return _symNameMap[offset];
    }
    return NULL;
}

const char *FileImplLibelf::getRelocNameByFileOff(
        Elf64_Off start, Elf64_Off end)
{
    map<Elf64_Off, const char *>::const_iterator it =
        _relocNameMap.lower_bound(start);
    if(it != _relocNameMap.end() && it->first < end) {
        return it->second;
    }
    return NULL;
}

bool FileImplLibelf::getRel(size_t scnIdx, int idx, Elf64_Rel *dst)
{
    if(!checkScnData(scnIdx, &_relCache)) {
        return false;
    }
    GElf_Rel rel;
    if(!gelf_getrel(_relCache.data, idx, &rel)) {
        return false;
    }
    ConvertClass::cvt(*dst, rel);
    return true;
}

bool FileImplLibelf::getRela(size_t scnIdx, int idx, Elf64_Rela *dst)
{
    if(!checkScnData(scnIdx, &_relaCache)) {
        return false;
    }
    GElf_Rela rela;
    if(!gelf_getrela(_relaCache.data, idx, &rela)) {
        return false;
    }
    ConvertClass::cvt(*dst, rela);
    return true;
}

char *FileImplLibelf::findDynTag(Elf64_Sxword val)
{
    Elf64_Phdr phdr;
    size_t phdrNum;
    if(getPhdrNum(&phdrNum) != 0) {
        return NULL;
    }
    ConvertAddr conv(this);
    for(size_t i=0; i<phdrNum; i++) {
        if(!getPhdr(i, &phdr)) {
            continue;
        }
        if(phdr.p_type != PT_DYNAMIC) {
            continue;
        }
        char *dynRaw = getRawData(phdr.p_offset);
        size_t resOff;
        size_t entSize = getClass()==ELFCLASS32 ?
            sizeof(Elf32_Dyn) : sizeof(Elf64_Dyn);
        size_t entNum = phdr.p_filesz/entSize;
        for(size_t j=0; j<entNum; j++) {
            Elf64_Dyn dyn;
            ConvertClass::cvt(dyn, *(Elf64_Dyn*)(dynRaw+j*entSize));
            if(dyn.d_tag == val) {
                if(!conv.vaddrToFileOff(resOff, dyn.d_un.d_ptr)) {
                    return NULL;
                }
                return getRawData(resOff);
            }
        }
    }
    return NULL;
}

size_t FileImplLibelf::detectDynSymCnt()
{
    if(!_hashTabRaw && !(_hashTabRaw=findDynTag(DT_GNU_HASH))) {
        return 0;
    }
    if(getClass() != ELFCLASS32 && getClass() != ELFCLASS64) {
        return 0;
    }
    size_t C = getClass()==ELFCLASS32 ? 4 : 8;
    uint32_t nbuckets = *(uint32_t*)_hashTabRaw;
    uint32_t symndx = *((uint32_t*)_hashTabRaw+1);
    uint32_t maskwords = *((uint32_t*)_hashTabRaw+2);
    char *bloom = _hashTabRaw+sizeof(uint32_t)*4;
    uint32_t *buckets = (uint32_t*)(bloom+maskwords*C);
    uint32_t *hashvals = (uint32_t*)(buckets+nbuckets);
    uint32_t *hv;

    size_t cnt = symndx;
    for(size_t i=0; i<nbuckets; i++) {
        if(buckets[i]==0) {
            continue;
        }
        hv = &hashvals[buckets[i]-symndx];
        do {
            cnt++;
        } while(((*hv++) & 1) == 0);
    }
    return cnt;
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
                ConvertClass::cvt(*dst, *sym);
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
    DisasmPrivData *priv = (DisasmPrivData *)info->callerData;
    if(info->labelBuf) {
        free(info->labelBuf);
        info->labelBuf = NULL;
    }
    map<Elf64_Off, const char *> *symNameMap = priv->symNameMap;
    Elf64_Off fileOff = 0;
    if(info->convertAddr->vaddrToFileOff(fileOff, val) &&
            symNameMap->find(fileOff) != symNameMap->end())
    {
        info->labelBuf = strdup((*symNameMap)[fileOff]);
        *size = strlen(info->labelBuf);
    }
    *buf = info->labelBuf;
    return 0;
}

void FileImplLibelf::prepareSymLookup()
{
    _symNameMap.clear();
    ConvertAddr convertAddr(this);
    char *dynSymRaw = findDynTag(DT_SYMTAB);
    char *dynStrRaw = findDynTag(DT_STRTAB);
    size_t dynSymNum = detectDynSymCnt();
    for(size_t i=0; i<dynSymNum; i++) {
        Elf64_Sym sym;
        if(getClass() == ELFCLASS32) {
            ConvertClass::cvt(sym, *((Elf32_Sym*)dynSymRaw+i));
        } else if(getClass() == ELFCLASS64) {
            ConvertClass::cvt(sym, *((Elf64_Sym*)dynSymRaw+i));
        } else {
            break;
        }

        if(sym.st_shndx == 0) {
            continue;
        }
        Elf64_Off fileOff = 0;
        if(ELF64_ST_TYPE(sym.st_info) == STT_FILE ||
                !convertAddr.vaddrToFileOff(fileOff, sym.st_value))
        {
            continue;
        }
        if(_symDataMap.find(fileOff) == _symDataMap.end()) {
            _symDataMap[fileOff] = sym;
        }

        if(dynStrRaw[sym.st_name] == 0) {
            continue;
        }
        if(_symNameMap.find(fileOff) == _symNameMap.end()) {
            _symNameMap[fileOff] = dynStrRaw+sym.st_name;
        }
    }

    Elf64_Ehdr ehdr;
    size_t shdrNum;
    if(getShdrNum(&shdrNum)!=0 || !getEhdr(&ehdr)) {
        return;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!getShdr(i, &shdr) ||
                (shdr.sh_type != SHT_SYMTAB &&
                 shdr.sh_type != SHT_DYNSYM))
        {
            continue;
        }
        for(size_t symIdx = 0;
                symIdx < shdr.sh_size/shdr.sh_entsize; symIdx++)
        {
            Elf64_Sym sym;
            if(!getSym(i, symIdx, &sym) || sym.st_shndx == 0 ||
                    ELF64_ST_TYPE(sym.st_info) == STT_FILE)
            {
                continue;
            }
            bool ok;
            Elf64_Off fileOff = 0;
            if(ehdr.e_type == ET_REL) {
                ok = convertAddr.secOffToFileOff(fileOff,
                        sym.st_shndx, sym.st_value);
            } else {
                ok = convertAddr.vaddrToFileOff(fileOff, sym.st_value);
            }
            if(!ok) {
                continue;
            }
            if(_symDataMap.find(fileOff) == _symDataMap.end()) {
                _symDataMap[fileOff] = sym;
            }
            if(getStrPtr(shdr.sh_link, sym.st_name)[0] == 0) {
                continue;
            }
            if(_symNameMap.find(fileOff) == _symNameMap.end()) {
                _symNameMap[fileOff] =
                    getStrPtr(shdr.sh_link, sym.st_name);
            }
        }
    }
}

int FileImplLibelf::disasmOutput(char *buf, size_t len, void *arg)
{
    DisasmCBInfo *info = (DisasmCBInfo *)arg;
    DisasmPrivData *priv = (DisasmPrivData *)info->callerData;
    return priv->outputCB(buf, len, arg);
}

bool FileImplLibelf::checkScnData(size_t scnIdx, ScnDataCache *cache)
{
    if(scnIdx != cache->idx) {
        Elf_Scn *scn = elf_getscn(_elf, scnIdx);
        Elf_Data *data = NULL;
        if(!scn || (data=elf_getdata(scn, data))==NULL) {
            return false;
        }
        cache->idx = scnIdx;
        cache->data = data;
    }
    return true;
}

void FileImplLibelf::prepareRelocLookup()
{
    _relocNameMap.clear();
    ConvertAddr convertAddr(this);
    Elf64_Ehdr ehdr;
    size_t shdrNum;
    if(getShdrNum(&shdrNum) != 0 || !getEhdr(&ehdr)) {
        return;
    }
    for(size_t i=0; i<shdrNum; i++) {
        Elf64_Shdr shdr;
        if(!getShdr(i, &shdr)) {
            continue;
        }
        if(shdr.sh_type != SHT_REL && shdr.sh_type != SHT_RELA) {
            continue;
        }
        for(size_t ent=0; ent<shdr.sh_size/shdr.sh_entsize; ent++) {
            union {
                Elf64_Rel rel;
                Elf64_Rela rela;
            } rel;
            if(shdr.sh_type == SHT_REL) {
                if(!getRel(i, ent, &rel.rel)) {
                    continue;
                }
            } else if(shdr.sh_type == SHT_RELA) {
                if(!getRela(i, ent, &rel.rela)) {
                    continue;
                }
            } else {
                continue;
            }
            Elf64_Sym sym;
            if(!getSym(shdr.sh_link,
                        ELF64_R_SYM(rel.rel.r_info), &sym))
            {
                continue;
            }
            Elf64_Shdr symShdr;
            if(!getShdr(shdr.sh_link, &symShdr)) {
                continue;
            }
            const char *symName = getStrPtr(symShdr.sh_link, sym.st_name);
            bool ok;
            Elf64_Off fileOff = 0;
            if(ehdr.e_type == ET_REL) {
                ok = convertAddr.secOffToFileOff(fileOff,
                        shdr.sh_info, rel.rel.r_offset);
            } else {
                ok = convertAddr.vaddrToFileOff(fileOff,
                        rel.rel.r_offset);
            }
            if(ok && _relocNameMap.find(fileOff) == _relocNameMap.end())
            {
                _relocNameMap[fileOff] = symName;
            }
        }
    }
}

END_BIN_NAMESPACE
