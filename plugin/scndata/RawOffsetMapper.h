#ifndef PLUGIN_SCNDATA_RAW_OFFSET_MAPPER_H
#define PLUGIN_SCNDATA_RAW_OFFSET_MAPPER_H

#include <QtGui/QTextDocument>

#include "common.h"

#include "OffsetMapper.h"

BEGIN_PLUG_NAMESPACE(scndata)

class RawOffsetMapper : public OffsetMapper {
public:
    RawOffsetMapper(QTextDocument * = NULL);
    virtual ~RawOffsetMapper() {}
    virtual void toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd);
    virtual void fromOffset(int &curStart, int &curEnd,
            int offStart, int offEnd);
private:
    QTextDocument *_td;
};

END_PLUG_NAMESPACE

#endif
