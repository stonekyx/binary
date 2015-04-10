#include "HexOffsetMapper.h"

BEGIN_PLUG_NAMESPACE(scndata)

void HexOffsetMapper::toOffset(int &offStart, int &offEnd,
            int curStart, int curEnd)
{
    offStart = curStart/(16*3)*16 + curStart%(16*3)/3;
    offEnd = curEnd/(16*3)*16 + curEnd%(16*3)/3 + (curEnd%3>0);
    if(curStart != curEnd) {
        offStart += (curStart%3 >= 2);
    } else {
        offEnd += (curEnd%3==0);
    }
}

void HexOffsetMapper::fromOffset(int &curStart, int &curEnd,
        int offStart, int offEnd)
{
    curStart = offStart*3;
    curEnd = offEnd*3;
    if(curStart != curEnd) {
        curEnd --;
    }
}

END_PLUG_NAMESPACE
