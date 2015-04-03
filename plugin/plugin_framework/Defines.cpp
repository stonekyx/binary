#include <cstring>

#include "Defines.h"

#include "defines/all.h"

using namespace std;
BEGIN_PLUG_NAMESPACE(plugin_framework)

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

END_PLUG_NAMESPACE
