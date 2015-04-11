#ifndef PLUGIN_SCNDATA_RAW_OFFSET_MAPPER_H
#define PLUGIN_SCNDATA_RAW_OFFSET_MAPPER_H

#include "common.h"

#include "OffsetMapper.h"

BEGIN_PLUG_NAMESPACE(scndata)

class RawOffsetMapper : public OffsetMapper {
public:
    virtual ~RawOffsetMapper() {}
    virtual void toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd);
    virtual void fromOffset(int &curStart, int &curEnd,
            int offStart, int offEnd);
};

END_PLUG_NAMESPACE

#endif
