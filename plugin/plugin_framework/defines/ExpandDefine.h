#ifndef PLUGIN_PLUGIN_FRAMEWORK_EXPAND_DEFINE_H
#define PLUGIN_PLUGIN_FRAMEWORK_EXPAND_DEFINE_H

#include <map>
#include <set>
#include <utility>
#include <cstdlib>

#include "common.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

struct DefineInfo {
    DefineInfo(bool valid=true) : valid(valid) {name = explain = NULL;}
    const char *name;
    const char *explain;
    bool valid;
};

template<typename T>
class ExpandDefine {
public:
    std::map<T, DefineInfo> defineMap;
    std::map<std::pair<T, T>, DefineInfo> rangeMap;
    std::set<T> rangeSet;
    DefineInfo nullInfo;

    typedef typename std::set<T>::const_iterator RangeSetIr;
    typedef typename std::pair<RangeSetIr, RangeSetIr> RangeSetIrPair;
    typedef typename std::pair<T, T> RangeMapKey;
    typedef typename std::map<std::pair<T, T>, DefineInfo>::const_iterator RangeMapIr;

    ExpandDefine() : nullInfo(false) {}
    const DefineInfo &queryRange(T) const;
    const DefineInfo &queryRangeSlow(T) const;
};

template<typename T>
const DefineInfo &ExpandDefine<T>::queryRange(T val) const
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

template<typename T>
const DefineInfo &ExpandDefine<T>::queryRangeSlow(T val) const
{
    RangeMapKey resRng;
    const DefineInfo *res = &nullInfo;
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
