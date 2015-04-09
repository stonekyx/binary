#ifndef PLUGIN_PLUGIN_FRAMEWORK_EXPAND_DEFINE_H
#define PLUGIN_PLUGIN_FRAMEWORK_EXPAND_DEFINE_H

#include <map>
#include <set>
#include <utility>
#include <cstdlib>

#include "common.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

template<typename T=const char>
struct DefineInfo {
    DefineInfo(bool valid=true) : name(NULL),explain(NULL),valid(valid) {}
    const char *name;
    T *explain;
    bool valid;
};

template<typename T, typename U=const char>
class ExpandDefine {
public:
    std::map<T, DefineInfo<U> > defineMap;
    std::map<std::pair<T, T>, DefineInfo<U> > rangeMap;
    std::set<T> rangeSet;
    DefineInfo<U> nullInfo;

    typedef U InfoType;
    typedef typename std::set<T>::const_iterator RangeSetIr;
    typedef typename std::pair<RangeSetIr, RangeSetIr> RangeSetIrPair;
    typedef typename std::pair<T, T> RangeMapKey;
    typedef typename std::map<std::pair<T, T>, DefineInfo<U> >::const_iterator RangeMapIr;

    ExpandDefine() : nullInfo(false) {}
    const DefineInfo<U> &queryRange(T) const;
    const DefineInfo<U> &queryRangeSlow(T) const;
};

template<typename T, typename U>
const DefineInfo<U> &ExpandDefine<T, U>::queryRange(T val) const
{
    RangeSetIrPair rangeIr = rangeSet.equal_range(val);
    if(rangeIr.first == rangeSet.end() ||
            rangeIr.second == rangeSet.end())
    {
        return nullInfo;
    }
    RangeMapKey key(*rangeIr.first, *rangeIr.second);
    if(rangeMap.find(key) == rangeMap.end()) {
        return nullInfo;
    }
    return rangeMap.find(key)->second;
}

template<typename T, typename U>
const DefineInfo<U> &ExpandDefine<T, U>::queryRangeSlow(T val) const
{
    RangeMapKey resRng;
    const DefineInfo<U> *res = &nullInfo;
    for(RangeMapIr it = rangeMap.begin(); it!=rangeMap.end(); it++) {
        if(it->first.first<=val && it->first.second>=val) {
            if(!res->valid || resRng.second-resRng.first > it->first.second-it->first.first)
            {
                resRng = it->first;
                res = &it->second;
            }
        }
    }
    return *res;
}

END_PLUG_NAMESPACE

#endif
