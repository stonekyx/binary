#ifndef PLUGIN_PLUGIN_FRAMEWORK_DEFINES_H
#define PLUGIN_PLUGIN_FRAMEWORK_DEFINES_H

#include <elf.h>
#include <vector>
#include <cstdlib>

#include "common.h"

#include "defines/all-extern.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

class Defines {
public:
    static std::vector<const char *> macroText_SHF(Elf64_Xword);
    static std::vector<const char *> commentText_SHF(Elf64_Xword);
    static const char *macroText_SHT(Elf64_Word);
    static const char *commentText_SHT(Elf64_Word);
    static size_t commentText_PF(Elf64_Word, char*);
    template<typename T>
    static const char *anyText(const T &, const ExpandDefine<T> &,
            const char *DefineInfo::*memb);
    template<typename T>
    static const char *macroText(const T &, const ExpandDefine<T> &);
    template<typename T>
    static const char *commentText(const T &, const ExpandDefine<T> &);
};

template<typename T>
const char *Defines::anyText(const T &val, const ExpandDefine<T> &obj,
        const char *DefineInfo::*memb)
{
    if(obj.defineMap.find(val) != obj.defineMap.end()) {
        return obj.defineMap.find(val)->second.*memb;
    }
    const DefineInfo &info = obj.queryRange(val);
    if(info.valid) {
        return info.*memb;
    }
    return "Unknown";
}

template<typename T>
const char *Defines::macroText(const T &val, const ExpandDefine<T> &obj)
{
    return anyText(val, obj, &DefineInfo::name);
}

template<typename T>
const char *Defines::commentText(const T &val,
        const ExpandDefine<T> &obj)
{
    return anyText(val, obj, &DefineInfo::explain);
}

END_PLUG_NAMESPACE

#endif
