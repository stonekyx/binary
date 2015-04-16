#ifndef PLUGIN_FRAMEWORK_CTX_JUDGE_H
#define PLUGIN_FRAMEWORK_CTX_JUDGE_H

#include <cstdlib>

#include "common.h"

class QModelIndex;

BEGIN_PLUG_NAMESPACE(plugin_framework)

class CtxAction;
class CtxJudge {
public:
    typedef bool (*JudgeFunc)(CtxAction *, const QModelIndex &);
    CtxJudge(JudgeFunc = NULL);
    virtual ~CtxJudge();
    virtual bool operator()(CtxAction *, const QModelIndex &);
private:
    JudgeFunc _func;
};

END_PLUG_NAMESPACE

#endif