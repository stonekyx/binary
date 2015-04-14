#include "CtxJudge.h"
#include "CtxAction.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

CtxAction::CtxAction(CtxJudge *judge, QWidget *parent) :
    QAction(parent), _judge(judge)
{
}

CtxAction::~CtxAction()
{
    if(_judge) {
        delete _judge;
    }
}

bool CtxAction::judgeVisible(const QModelIndex &index)
{
    if(!_judge || (*_judge)(index)) {
        return true;
    }
    return false;
}

END_PLUG_NAMESPACE
