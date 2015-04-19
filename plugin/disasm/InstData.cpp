#include <cstdlib>
#include <cstring>

#include "InstData.h"

BEGIN_PLUG_NAMESPACE(disasm)

InstData::InstData()
{
    memset(&d, 0, sizeof(d));
    addrType = AT_NONE;
}

InstData::InstData(const InstData &o)
{
    *this = o;
}

InstData &InstData::operator=(const InstData &o)
{
    addrType = o.addrType;
    d = o.d;
    switch(addrType) {
    case AT_SYMBOL:
        d.symbol.symName = strdup(o.d.symbol.symName); break;
    case AT_RELOC:
        d.reloc.symName = strdup(o.d.reloc.symName); break;
    default:
        break;
    }
    return *this;
}

InstData::~InstData()
{
    switch(addrType) {
    case AT_SYMBOL:
        free(d.symbol.symName); break;
    case AT_RELOC:
        free(d.reloc.symName); break;
    default:
        break;
    }
}

END_PLUG_NAMESPACE
