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
    template<typename T, typename U, typename V>
    static V *anyText(const T &, const ExpandDefine<T, U> &,
            V *DefineInfo<U>::*memb, V *failsafe);
    template<typename T, typename U>
    static const char *macroText(const T &, const ExpandDefine<T, U> &);
    template<typename T, typename U>
    static U *commentText(const T &, const ExpandDefine<T, U> &);
    template<typename T>
    static const char *commentText(const T &, const ExpandDefine<T> &);
};

template<typename T, typename U, typename V>
V *Defines::anyText(const T &val, const ExpandDefine<T, U> &obj,
        V *DefineInfo<U>::*memb, V *failsafe)
{
    if(obj.defineMap.find(val) != obj.defineMap.end()) {
        return obj.defineMap.find(val)->second.*memb;
    }
    const DefineInfo<U> &info = obj.queryRange(val);
    if(info.valid) {
        return info.*memb;
    }
    return failsafe;
}

template<typename T, typename U>
const char *Defines::macroText(const T &val, const ExpandDefine<T, U> &obj)
{
    return anyText(val, obj, &DefineInfo<U>::name, "Unknown");
}

template<typename T, typename U>
U *Defines::commentText(const T &val,
        const ExpandDefine<T, U> &obj)
{
    return anyText(val, obj, &DefineInfo<U>::explain, (U*)NULL);
}

template<typename T>
const char *Defines::commentText(const T &val, const ExpandDefine<T> &obj)
{
    return anyText(val, obj, &DefineInfo<>::explain, "Unknown");
}

END_PLUG_NAMESPACE

#endif
