#include "CtxJudge.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

CtxJudge::CtxJudge(JudgeFunc func) : _func(func)
{
}

CtxJudge::~CtxJudge()
{
}

bool CtxJudge::operator()(CtxAction *action, const QModelIndex &index)
{
    if(_func) {
        return _func(action, index);
    }
    return true;
}

END_PLUG_NAMESPACE
