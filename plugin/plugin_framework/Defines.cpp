#include <cstring>

#include "Defines.h"

#include "defines/all.h"

using namespace std;
BEGIN_PLUG_NAMESPACE(plugin_framework)

template<typename T>
const char *anyText(T val,
        ExpandDefine<T> &obj, const char *DefineInfo::*memb)
{
    if(obj.defineMap.find(val) != obj.defineMap.end()) {
        return obj.defineMap[val].*memb;
    }
    const DefineInfo &info = obj.queryRange(val);
    if(info.valid) {
        return info.*memb;
    }
    return "Unknown";
}

template<typename T>
const char *anyTextSlow(T val,
        ExpandDefine<T> &obj, const char *DefineInfo::*memb)
{
    if(obj.defineMap.find(val) != obj.defineMap.end()) {
        return obj.defineMap[val].*memb;
    }
    const DefineInfo &info = obj.queryRangeSlow(val);
    if(info.valid) {
        return info.*memb;
    }
    return "Unknown";
}

const char *Defines::macroText_DT(Elf64_Sxword tag)
{
    return anyText<Elf64_Sxword>(tag, defines_DT, &DefineInfo::name);
}

const char *Defines::commentText_DT(Elf64_Sxword tag)
{
    return anyText<Elf64_Sxword>(tag, defines_DT, &DefineInfo::explain);
}

vector<const char *> Defines::macroText_SHF(Elf64_Xword val)
{
    vector<const char *> res;
    for(map<Elf64_Xword, DefineInfo>::iterator it = defines_SHF.defineMap.begin();
            it != defines_SHF.defineMap.end(); it++)
    {
        if(val & it->first) {
            res.push_back(it->second.name);
        }
    }
    return res;
}

vector<const char *> Defines::commentText_SHF(Elf64_Xword val)
{
    vector<const char *> res;
    for(map<Elf64_Xword, DefineInfo>::iterator it = defines_SHF.defineMap.begin();
            it != defines_SHF.defineMap.end(); it++)
    {
        if(val & it->first) {
            res.push_back(it->second.explain);
        }
    }
    return res;
}

const char *Defines::macroText_SHT(Elf64_Word val)
{
    return anyTextSlow<Elf64_Word>(val, defines_SHT, &DefineInfo::name);
}

const char *Defines::commentText_SHT(Elf64_Word val)
{
    return anyTextSlow<Elf64_Word>(val, defines_SHT, &DefineInfo::explain);
}

const char *Defines::macroText_STB(unsigned char val)
{
    return anyText<unsigned char>(val, defines_STB, &DefineInfo::name);
}

const char *Defines::commentText_STB(unsigned char val)
{
    return anyText<unsigned char>(val, defines_STB, &DefineInfo::explain);
}

const char *Defines::macroText_STT(unsigned char val)
{
    return anyText<unsigned char>(val, defines_STT, &DefineInfo::name);
}

const char *Defines::commentText_STT(unsigned char val)
{
    return anyText<unsigned char>(val, defines_STT, &DefineInfo::explain);
}

const char *Defines::macroText_STV(unsigned char val)
{
    return anyText<unsigned char>(val, defines_STV, &DefineInfo::name);
}

const char *Defines::commentText_STV(unsigned char val)
{
    return anyText<unsigned char>(val, defines_STV, &DefineInfo::explain);
}

const char *Defines::macroText_PT(Elf64_Word val)
{
    return anyText<Elf64_Word>(val, defines_PT, &DefineInfo::name);
}

const char *Defines::commentText_PT(Elf64_Word val)
{
    return anyText<Elf64_Word>(val, defines_PT, &DefineInfo::explain);
}

size_t Defines::commentText_PF(Elf64_Word val, char *dst)
{
    static char buf[] = "rwx, OS-specific, Processor-specific";
    memset(buf, 0, sizeof(buf));
    memset(buf, '-', 3);
    if((val & PF_R)) {
        buf[0] = 'r';
    }
    if((val & PF_W)) {
        buf[1] = 'w';
    }
    if((val & PF_X)) {
        buf[2] = 'x';
    }
    if((val & PF_MASKOS)) {
        strncat(buf, ", OS-specific", sizeof(buf)-strlen(buf)-1);
    }
    if((val & PF_MASKPROC)) {
        strncat(buf, ", Processor-specific", sizeof(buf)-strlen(buf)-1);
    }
    if(dst) {
        strcpy(dst, buf);
    }
    return strlen(buf);
}

const char *Defines::macroText_EM(Elf64_Half val)
{
    return anyText<Elf64_Half>(val, defines_EM, &DefineInfo::name);
}

const char *Defines::commentText_EM(Elf64_Half val)
{
    return anyText<Elf64_Half>(val, defines_EM, &DefineInfo::explain);
}

const char *Defines::macroText_ET(Elf64_Half val)
{
    return anyText<Elf64_Half>(val, defines_ET, &DefineInfo::name);
}

const char *Defines::commentText_ET(Elf64_Half val)
{
    return anyText<Elf64_Half>(val, defines_ET, &DefineInfo::explain);
}

const char *Defines::macroText_EV(Elf64_Word val)
{
    return anyText<Elf64_Word>(val, defines_EV, &DefineInfo::name);
}

const char *Defines::commentText_EV(Elf64_Word val)
{
    return anyText<Elf64_Word>(val, defines_EV, &DefineInfo::explain);
}

const char *Defines::macroText_ELFCLASS(int val)
{
    return anyText<int>(val, defines_ELFCLASS, &DefineInfo::name);
}

const char *Defines::commentText_ELFCLASS(int val)
{
    return anyText<int>(val, defines_ELFCLASS, &DefineInfo::explain);
}

END_PLUG_NAMESPACE
