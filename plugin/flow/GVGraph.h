#ifndef PLUGIN_FLOW_GVGRAPH_H
#define PLUGIN_FLOW_GVGRAPH_H

#include <QtCore/QMultiMap>
#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtGui/QFont>
#include <QtGui/QPainterPath>
#include <QtGui/QPaintDevice>

#include <gvc.h>

#include "common.h"

BEGIN_PLUG_NAMESPACE(flow)

/// A struct containing the information for a GVGraph's node
struct GVNode
{
    /// The unique identifier of the node in the graph
    QString name;

    /// The position of the center point of the node from the top-left corner
    QPoint centerPos;

    /// The size of the node in pixels
    double height, width;

    QString label;
    QString comment;
};

/// A struct containing the information for a GVGraph's edge
struct GVEdge
{
    /// The source and target nodes of the edge
    QString source;
    QString target;

    /// Path of the edge's line
    QPainterPath path;

    struct label {
        QString text;
        QPointF pos;
        QFont font;
    } head_label, tail_label;
    QPolygonF arrowhead;
};

/// An object containing a libgraph graph and its associated nodes and edges
class GVGraph
{
public:
    typedef enum {
        GV_FDP,
        GV_DOT,
    } GVAlgoType;

    /// Default DPI value used by dot (which uses points instead of pixels for coordinates)
    static const qreal DotDefaultDPI;

    /*!
     * \brief Construct a Graphviz graph object
     * \param name The name of the graph, must be unique in the application
     * \param font The font to use for the graph
     * \param node_size The size in pixels of each node
     */
    GVGraph(GVAlgoType algo, QString name, QFont font, QPaintDevice *);
    ~GVGraph();
    void parseDot(const QString &);

    /// Add and remove nodes
    void addNode(const QString& name);
    void addNodes(const QStringList& names);
    void removeNode(const QString& name);
    void clearNodes();
    void setRootNode(const QString &);

    /// Add and remove edges
    void addEdge(const QString& source, const QString& target);
    void removeEdge(const QString& source, const QString& target);
    void removeEdge(const QPair<QString, QString> &);

    /// Set the font to use in all the labels
    void setFont(QFont font);

    void applyLayout();
    QRectF boundingRect() const;
    QList<GVNode> nodes() const;
    QList<GVEdge> edges() const;

private:
    GVC_t *_context;
    Agraph_t *_graph;
    QFont _font;
    QMap<QString, Agnode_t*> _nodes;
    QMultiMap<QPair<QString, QString>, Agedge_t*> _edges;
    QPaintDevice *_pd;
    GVAlgoType _algo;
};

END_PLUG_NAMESPACE

#endif
