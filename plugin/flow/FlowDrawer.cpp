#include <gvc.h>
#include <cgraph.h>

#include <QtGui/QGraphicsTextItem>
#include <QtGui/QFontMetrics>

#include "GVGraph.h"

#include "FlowDrawer.h"

using namespace std;
BEGIN_PLUG_NAMESPACE(flow)

FlowDrawer::FlowDrawer(QPaintDevice *pd) : _pd(pd)
{
}

void FlowDrawer::draw(QGraphicsScene *scene, const string &src)
{
    QFont font("Courier", 14.0);
    GVGraph graph("G", font, _pd);
    QString qsrc = QString::fromStdString(src);
    graph.parseDot(qsrc);
    graph.applyLayout();
    QList<GVNode> nodes = graph.nodes();
    foreach(GVNode node, nodes) {
        QPointF topleft(node.centerPos);
        topleft.setX(topleft.x() - node.width/2.0);
        topleft.setY(topleft.y() - node.height/2.0);
        scene->addRect(topleft.x(), topleft.y(), node.width, node.height);
        scene->addText(node.label.replace("\\l", "\n"), font)
            ->setPos(topleft);
    }
    QList<GVEdge> edges = graph.edges();
    foreach(const GVEdge &edge, edges) {
        scene->addPath(edge.path);
        if(!edge.head_label.text.isEmpty()) {
            scene->addText(edge.head_label.text, edge.head_label.font)
                ->setPos(edge.head_label.pos);
        }
        if(!edge.tail_label.text.isEmpty()) {
            scene->addText(edge.tail_label.text, edge.tail_label.font)
                ->setPos(edge.tail_label.pos);
        }
        scene->addPolygon(edge.arrowhead, QPen(), QBrush(Qt::black));
    }
}

END_PLUG_NAMESPACE
