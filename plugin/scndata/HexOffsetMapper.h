#ifndef PLUGIN_SCNDATA_HEX_OFFSET_MAPPER_H
#define PLUGIN_SCNDATA_HEX_OFFSET_MAPPER_H

#include "common.h"

#include "OffsetMapper.h"

BEGIN_PLUG_NAMESPACE(scndata)

class HexOffsetMapper : public OffsetMapper {
public:
    virtual ~HexOffsetMapper() {}
    virtual void toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd);
    virtual void fromOffset(int &curStart, int &curEnd,
            int offStart, int offEnd);
};

END_PLUG_NAMESPACE

#endif