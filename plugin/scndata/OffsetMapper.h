#ifndef PLUGIN_SCNDATA_OFFSET_MAPPER_H
#define PLUGIN_SCNDATA_OFFSET_MAPPER_H

#include "common.h"

BEGIN_PLUG_NAMESPACE(scndata)

class OffsetMapper {
public:
    virtual void toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd) = 0;
    virtual void fromOffset(int &curStart, int &curEnd,
            int offStart, int offEnd) = 0;
};

END_PLUG_NAMESPACE

#endif
