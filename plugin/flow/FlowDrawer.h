#ifndef PLUGIN_FLOW_FLOWDRAWER_H
#define PLUGIN_FLOW_FLOWDRAWER_H

#include <string>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPaintDevice>

#include "common.h"
#include "GVGraph.h"

BEGIN_PLUG_NAMESPACE(flow)

class FlowDrawer {
public:
    FlowDrawer(GVGraph::GVAlgoType algo, QPaintDevice *);
    void draw(QGraphicsScene *, const std::string &);
    typedef enum {
        ITEMKEY_NB,
        ITEMKEY_COLOR,
        ITEMKEY_RECT,
    } ItemKey;
private:
    QPaintDevice *_pd;
    GVGraph::GVAlgoType _algo;
};

END_PLUG_NAMESPACE

#endif
