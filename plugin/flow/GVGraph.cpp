#include <cmath>

#include <types.h>

#include <QtCore/QStringList>
#include <QtGui/QFontMetrics>

#include "GVWrapper.h"
#include "GVGraph.h"

BEGIN_PLUG_NAMESPACE(flow)

static inline double sqr(double x) {
    return x*x;
}

/*! Dot uses a 72 DPI value for converting it's position coordinates from points to pixels
    while we display at 96 DPI on most operating systems. */
const qreal GVGraph::DotDefaultDPI=72.0;

GVGraph::GVGraph(GVAlgoType algo, QString name, QFont font, QPaintDevice *pd) :
        _context(gvContext()),
#ifdef WITH_CGRAPH
        _graph(_agopen(name, Agdirected)),
#else
        _graph(_agopen(name, AGDIGRAPH)),
#endif
        _pd(pd),
        _algo(algo)
{
    setFont(font);
}

GVGraph::~GVGraph()
{
    gvFreeLayout(_context, _graph);
    agclose(_graph);
    gvFreeContext(_context);
}

void GVGraph::parseDot(const QString &src)
{
    agclose(_graph);
    _graph = agmemread(src.toUtf8().data());
    QMap<Agnode_t*, QString> nameMap;
    QMap<Agedge_t*, QPair<Agnode_t*, Agnode_t*> > edgeMap;
    int nodeCnt = 0;
    for(Agnode_t *node = agfstnode(_graph);
            node; node = agnxtnode(_graph, node))
    {
        nameMap[node] = QString::number(nodeCnt++);
        for(Agedge_t *edge = agfstout(_graph, node);
                edge; edge = agnxtout(_graph, edge))
        {
            edgeMap[edge] = QPair<Agnode_t*, Agnode_t*>(
#ifdef WITH_CGRAPH
                    node, edge->node
#else
                    edge->head, edge->tail
#endif
                    );
        }
    }
    for(QMap<Agedge_t*, QPair<Agnode_t*, Agnode_t*> >::const_iterator
            it = edgeMap.begin(); it != edgeMap.end(); it++)
    {
        QPair<QString, QString> key(
                nameMap[it.value().first],
                nameMap[it.value().second]);
        _edges.insert(key, it.key());
    }
    for(QMap<Agnode_t*, QString>::const_iterator it = nameMap.begin();
            it != nameMap.end(); it++)
    {
        _nodes[it.value()] = it.key();
    }
}

void GVGraph::addNode(const QString& name)
{
    if(_nodes.contains(name))
        removeNode(name);

    _nodes.insert(name, _agnode(_graph, name));
}

void GVGraph::addNodes(const QStringList& names)
{
    for(int i=0; i<names.size(); ++i)
        addNode(names.at(i));
}

void GVGraph::removeNode(const QString& name)
{
    if(_nodes.contains(name))
    {
        agdelete(_graph, _nodes[name]);
        _nodes.remove(name);

        QList<QPair<QString, QString> >keys=_edges.keys();
        for(int i=0; i<keys.size(); ++i)
            if(keys.at(i).first==name || keys.at(i).second==name)
                removeEdge(keys.at(i));
    }
}

void GVGraph::clearNodes()
{
    QList<QString> keys=_nodes.keys();

    for(int i=0; i<keys.size(); ++i)
        removeNode(keys.at(i));
}

void GVGraph::setRootNode(const QString& name)
{
    if(_nodes.contains(name))
        _agset(_graph, "root", name);
}

void GVGraph::addEdge(const QString &source, const QString &target)
{
    if(_nodes.contains(source) && _nodes.contains(target))
    {
        QPair<QString, QString> key(source, target);
        _edges.insert(key, _agedge(_graph, _nodes[source], _nodes[target]));
    }
}

void GVGraph::removeEdge(const QString &source, const QString &target)
{
    removeEdge(QPair<QString, QString>(source, target));
}

void GVGraph::removeEdge(const QPair<QString, QString>& key)
{
    if(_edges.contains(key))
    {
        while(_edges.contains(key)) {
            Agedge_t *edge = _edges.find(key).value();
            agdelete(_graph, edge);
            _edges.remove(key, edge);
        }
    }
}

void GVGraph::setFont(QFont font)
{
    _font=font;

    _agset(_graph, "fontname", font.family());
    _agset(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

    _agnodeattr(_graph, "fontname", font.family());
    _agnodeattr(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

    _agedgeattr(_graph, "fontname", font.family());
    _agedgeattr(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));
}

void GVGraph::applyLayout()
{
    gvFreeLayout(_context, _graph);
    switch(_algo) {
    case GV_FDP:
        gvLayout(_context, _graph, "fdp");
        break;
    case GV_DOT:
        gvLayout(_context, _graph, "dot");
        break;
    default:
        break;
    }
}

QRectF GVGraph::boundingRect() const
{
    qreal dpiX=_pd->logicalDpiX();
    qreal dpiY=_pd->logicalDpiY();
    return QRectF(
            GD_bb(_graph).LL.x*(dpiX/DotDefaultDPI),
            GD_bb(_graph).LL.y*(dpiY/DotDefaultDPI),
            GD_bb(_graph).UR.x*(dpiX/DotDefaultDPI),
            GD_bb(_graph).UR.y*(dpiY/DotDefaultDPI));
}

QList<GVNode> GVGraph::nodes() const
{
    QList<GVNode> list;
    qreal dpiX=_pd->logicalDpiX();
    qreal dpiY=_pd->logicalDpiY();

    for(QMap<QString, Agnode_t*>::const_iterator it=_nodes.begin(); it!=_nodes.end();++it)
    {
        Agnode_t *node=it.value();
        GVNode object;

        //Set the name of the node
        object.name=it.key();

        //Fetch the X coordinate, apply the DPI conversion rate (actual DPI / 72, used by dot)
        qreal x=ND_coord(node).x*(dpiX/DotDefaultDPI);

        //Translate the Y coordinate from bottom-left to top-left corner
        qreal y=(GD_bb(_graph).UR.y - ND_coord(node).y)*(dpiY/DotDefaultDPI);
        object.centerPos=QPoint(x, y);

        //Transform the width and height from inches to pixels
        object.height=ND_height(node)*dpiY;
        object.width=ND_width(node)*dpiX;

        object.label = ND_label(node)->text;
        object.comment = _agget(node, "comment");

        list << object;
    }

    return list;
}

QList<GVEdge> GVGraph::edges() const
{
    QList<GVEdge> list;
    qreal dpiX=_pd->logicalDpiX();
    qreal dpiY=_pd->logicalDpiY();

    for(QMultiMap<QPair<QString, QString>, Agedge_t*>::const_iterator
            it=_edges.begin(); it!=_edges.end();++it)
    {
        Agedge_t *edge = it.value();
        GVEdge object;

        object.visible = true;

        //Fill the source and target node names
        object.source=it.key().first;
        object.target=it.key().second;

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)
        if((ED_spl(edge)->list!=0) && (ED_spl(edge)->list->size%3 == 1) &&
                _agget(edge, "style") != "invis")
        {
            //If there is a starting point, draw a line from it to the first curve point
            if(ED_spl(edge)->list->sflag)
            {
                object.path.moveTo(
                        ED_spl(edge)->list->sp.x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->sp.y)
                        *(dpiY/DotDefaultDPI));
                object.path.lineTo(
                        ED_spl(edge)->list->list[0].x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[0].y)
                        *(dpiY/DotDefaultDPI));
            }
            else
                object.path.moveTo(
                        ED_spl(edge)->list->list[0].x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[0].y)
                        *(dpiY/DotDefaultDPI));

            //Loop over the curve points
            for(int i=1; i<ED_spl(edge)->list->size; i+=3)
                object.path.cubicTo(
                        ED_spl(edge)->list->list[i].x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[i].y)
                        *(dpiY/DotDefaultDPI),
                        ED_spl(edge)->list->list[i+1].x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[i+1].y)
                        *(dpiY/DotDefaultDPI),
                        ED_spl(edge)->list->list[i+2].x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->list[i+2].y)
                        *(dpiY/DotDefaultDPI));

            //If there is an ending point, draw a line to it
            if(ED_spl(edge)->list->eflag) {
                QPointF top(ED_spl(edge)->list->ep.x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->ep.y)
                        *(dpiY/DotDefaultDPI));
                QPointF back(object.path.currentPosition());
                const double halfWidth = 5;
                double dist =
                    sqrt(sqr(top.x()-back.x()) + sqr(top.y()-back.y()));
                double cosVal = (top.x()-back.x()) / dist;
                double sinVal = (back.y()-top.y()) / dist;
                double dx = sinVal*halfWidth;
                double dy = cosVal*halfWidth;
                QPointF sideA(back);
                sideA.setX(sideA.x() + dx);
                sideA.setY(sideA.y() + dy);
                QPointF sideB(back);
                sideB.setX(sideB.x() - dx);
                sideB.setY(sideB.y() - dy);
                object.arrowhead << top << sideA << sideB << top;
                object.path.lineTo(
                        ED_spl(edge)->list->ep.x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->ep.y)
                        *(dpiY/DotDefaultDPI));
            }
        } else {
            object.visible = false;
        }

        if(ED_head_label(edge)) {
            textlabel_t *label = ED_head_label(edge);
            object.head_label.font = QFont("Times", label->fontsize);
            object.head_label.text = label->text;
            QFontMetrics fontMetrics(object.head_label.font);
            qreal x = label->pos.x;
            qreal y = GD_bb(_graph).UR.y - label->pos.y;
            x *= dpiX/DotDefaultDPI;
            y *= dpiY/DotDefaultDPI;
            x += label->space.x/2;
            y += label->space.y/2;
#ifdef TEXTPARA_H
            y -= label->u.txt.para->yoffset_centerline;
#elif defined(TEXTSPAN_H)
            y -= label->u.txt.span->yoffset_centerline;
#endif
            x -= fontMetrics.width(object.head_label.text);
            y -= fontMetrics.height();
            object.head_label.pos = QPointF(x, y);
        }
        if(ED_tail_label(edge)) {
            textlabel_t *label = ED_tail_label(edge);
            object.tail_label.font = QFont("Times", label->fontsize);
            object.tail_label.text = label->text;
            QFontMetrics fontMetrics(object.tail_label.font);
            qreal x = label->pos.x;
            qreal y = GD_bb(_graph).UR.y - label->pos.y;
            x *= dpiX/DotDefaultDPI;
            y *= dpiY/DotDefaultDPI;
            x += label->space.x/2;
            y += label->space.y/2;
#ifdef TEXTPARA_H
            y -= label->u.txt.para->yoffset_centerline;
#elif defined(TEXTSPAN_H)
            y -= label->u.txt.span->yoffset_centerline;
#endif
            x -= fontMetrics.width(object.tail_label.text);
            y -= fontMetrics.height();
            object.tail_label.pos = QPointF(x, y);
        }

        list << object;
    }

    return list;
}

END_PLUG_NAMESPACE
