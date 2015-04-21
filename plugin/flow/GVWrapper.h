#ifndef PLUGIN_FLOW_GVWRAPPER_H
#define PLUGIN_FLOW_GVWRAPPER_H

#include <cgraph.h>

#include <QtCore/QString>

#include "common.h"

BEGIN_PLUG_NAMESPACE(flow)

/// The agopen method for opening a graph
static inline Agraph_t* _agopen(QString name, Agdesc_t kind)
{
    return agopen(const_cast<char *>(qPrintable(name)), kind, 0);
}

/// Add an alternative value parameter to the method for getting an object's attribute
static inline QString _agget(void *object, QString attr, QString alt=QString())
{
    QString str=agget(object, const_cast<char *>(qPrintable(attr)));

    if(str==QString())
        return alt;
    else
        return str;
}

/// Directly use agsafeset which always works, contrarily to agset
static inline int _agset(void *object, QString attr, QString value)
{
    return agsafeset(object, const_cast<char *>(qPrintable(attr)),
                     const_cast<char *>(qPrintable(value)),
                     const_cast<char *>(qPrintable(value)));
}

static inline Agsym_t *_agnodeattr(Agraph_t *g, QString name, QString value)
{
    return agattr(g, AGNODE,
            const_cast<char*>(qPrintable(name)),
            const_cast<char*>(qPrintable(value)));
}

static inline Agsym_t *_agedgeattr(Agraph_t *g, QString name, QString value)
{
    return agattr(g, AGEDGE,
            const_cast<char*>(qPrintable(name)),
            const_cast<char*>(qPrintable(value)));
}

static inline Agnode_t *_agnode(Agraph_t *g, QString name, int createflag=0)
{
    return agnode(g, const_cast<char*>(qPrintable(name)), createflag);
}

static inline Agedge_t *_agedge(Agraph_t * g, Agnode_t * t, Agnode_t * h,
        QString name = QString(), int createflag = 0)
{
    return agedge(g, t, h, const_cast<char*>(qPrintable(name)), createflag);
}

END_PLUG_NAMESPACE

#endif
