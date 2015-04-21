#include <iostream>
#include <gvc.h>
#include <cgraph.h>

#include <QtGui/QGraphicsTextItem>
#include <QtGui/QFontMetrics>

#include "FlowDrawer.h"

using namespace std;
BEGIN_PLUG_NAMESPACE(flow)

FlowDrawer::FlowDrawer(QPaintDevice *pd) : _pd(pd)
{
}

void FlowDrawer::draw(QGraphicsScene *scene, const string &src)
{
    cout << src;
    GVC_t *gvc = gvContext();
    Agraph_t *G = agmemread(src.c_str());
    gvLayout(gvc, G, "fdp");
    gvRender(gvc, G, "dot", NULL);
    bool ok;
    QFont font("Courier", 14.0);
    for(Agnode_t *node = agfstnode(G); node; node = agnxtnode(G, node)) {
        double height = QString(agget(node, (char*)"height")).toDouble(&ok);
        if(!ok) continue;
        double width = QString(agget(node, (char*)"width")).toDouble(&ok);
        if(!ok) continue;
        QStringList coor = QString(agget(node, (char*)"pos")).split(",");
        if(coor.size() != 2) continue;
        double x = coor[0].toDouble(&ok);
        if(!ok) continue;
        double y = coor[1].toDouble(&ok);
        if(!ok) continue;
        //translate inch to points
        width *= _pd->logicalDpiX();
        height *= _pd->logicalDpiY();
        x = x * (_pd->logicalDpiX() / 72.0);
        y = y * (_pd->logicalDpiY() / 72.0);
        x -= width/2;
        y += height/2;
        scene->addRect(0, 0, width, height)->setPos(x, -y);
        QString label(agget(node, (char*)"label"));
        label.replace("\\l", "\n");
        scene->addText(label, font)->setPos(x, -y);
    }
}

END_PLUG_NAMESPACE
