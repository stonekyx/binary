#ifndef PLUGIN_FLOW_FLOWDRAWER_H
#define PLUGIN_FLOW_FLOWDRAWER_H

#include <string>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPaintDevice>

#include "common.h"

BEGIN_PLUG_NAMESPACE(flow)

class FlowDrawer {
public:
    FlowDrawer(QPaintDevice *);
    void draw(QGraphicsScene *, const std::string &);
private:
    QPaintDevice *_pd;
};

END_PLUG_NAMESPACE

#endif
