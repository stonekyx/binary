/*
 * Copyright 2015 KANG Yuxuan
 *
 * GVWrapper.h by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLUGIN_FLOW_GVWRAPPER_H
#define PLUGIN_FLOW_GVWRAPPER_H

#include <gvc.h>

#include <QtCore/QString>

#include "common.h"

BEGIN_PLUG_NAMESPACE(flow)

/// The agopen method for opening a graph
static inline Agraph_t* _agopen(QString name,
#ifdef WITH_CGRAPH
        Agdesc_t kind
#else
        int kind
#endif
        )
{
    return agopen(const_cast<char *>(qPrintable(name)), kind
#ifdef WITH_CGRAPH
            , 0
#endif
            );
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
#ifdef WITH_CGRAPH
    return agattr(g, AGNODE,
            const_cast<char*>(qPrintable(name)),
            const_cast<char*>(qPrintable(value)));
#else
    return agnodeattr(g,
            const_cast<char*>(qPrintable(name)),
            const_cast<char*>(qPrintable(value)));
#endif
}

static inline Agsym_t *_agedgeattr(Agraph_t *g, QString name, QString value)
{
#ifdef WITH_CGRAPH
    return agattr(g, AGEDGE,
            const_cast<char*>(qPrintable(name)),
            const_cast<char*>(qPrintable(value)));
#else
    return agedgeattr(g,
            const_cast<char*>(qPrintable(name)),
            const_cast<char*>(qPrintable(value)));
#endif
}

static inline Agnode_t *_agnode(Agraph_t *g, QString name
#ifdef WITH_CGRAPH
        , int createflag=0
#endif
        )
{
    return agnode(g, const_cast<char*>(qPrintable(name))
#ifdef WITH_CGRAPH
            , createflag
#endif
            );
}

static inline Agedge_t *_agedge(Agraph_t * g, Agnode_t * t, Agnode_t * h
#ifdef WITH_CGRAPH
        , QString name = QString(), int createflag = 0
#endif
        )
{
    return agedge(g, t, h
#ifdef WITH_CGRAPH
            , const_cast<char*>(qPrintable(name)), createflag
#endif
            );
}

END_PLUG_NAMESPACE

#endif
