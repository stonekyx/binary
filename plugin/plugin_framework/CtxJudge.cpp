#include "CtxJudge.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

CtxJudge::CtxJudge(JudgeFunc func) : _func(func)
{
}

CtxJudge::~CtxJudge()
{
}

bool CtxJudge::operator()(const QModelIndex &index)
{
    if(_func) {
        _func(index);
    }
    return true;
}

END_PLUG_NAMESPACE
