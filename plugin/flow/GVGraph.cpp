#include <types.h>

#include <QtCore/QStringList>

#include "GVWrapper.h"
#include "GVGraph.h"

BEGIN_PLUG_NAMESPACE(flow)

/*! Dot uses a 72 DPI value for converting it's position coordinates from points to pixels
    while we display at 96 DPI on most operating systems. */
const qreal GVGraph::DotDefaultDPI=72.0;

GVGraph::GVGraph(QString name, QFont font, QPaintDevice *pd) :
        _context(gvContext()),
        _graph(_agopen(name, Agdirected)),
        _pd(pd)
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
    _graph = agmemread(src.toUtf8().constData());
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
        if(!_edges.contains(key))
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
        agdelete(_graph, _edges[key]);
        _edges.remove(key);
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
    gvLayout(_context, _graph, "fdp");
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

        list << object;
    }

    return list;
}

QList<GVEdge> GVGraph::edges() const
{
    QList<GVEdge> list;
    qreal dpiX=_pd->logicalDpiX();
    qreal dpiY=_pd->logicalDpiY();

    for(QMap<QPair<QString, QString>, Agedge_t*>::const_iterator it=_edges.begin();
        it!=_edges.end();++it)
    {
        Agedge_t *edge=it.value();
        GVEdge object;

        //Fill the source and target node names
        object.source=ED_tail_port(edge).name;
        object.target=ED_head_port(edge).name;

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)
        if((ED_spl(edge)->list!=0) && (ED_spl(edge)->list->size%3 == 1))
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
            if(ED_spl(edge)->list->eflag)
                object.path.lineTo(
                        ED_spl(edge)->list->ep.x*(dpiX/DotDefaultDPI),
                        (GD_bb(_graph).UR.y - ED_spl(edge)->list->ep.y)
                        *(dpiY/DotDefaultDPI));
        }

        list << object;
    }

    return list;
}

END_PLUG_NAMESPACE
