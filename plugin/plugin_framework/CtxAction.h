#ifndef PLUGIN_FRAMEWORK_CTX_ACTION_H
#define PLUGIN_FRAMEWORK_CTX_ACTION_H

#include <QtGui/QAction>

#include "common.h"

class QModelIndex;

BEGIN_PLUG_NAMESPACE(plugin_framework)

class CtxJudge;
class CtxAction;

END_PLUG_NAMESPACE

class binary::plugin::plugin_framework::CtxAction :
    public QAction
{
public:
    CtxAction(CtxJudge * = NULL, QWidget * = NULL);
    ~CtxAction();
    bool judgeVisible(const QModelIndex &);
private:
    CtxJudge *_judge;
};

#endif
