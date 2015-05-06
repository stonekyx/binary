/*
 * Copyright 2015 KANG Yuxuan
 *
 * FlowDrawer.h by KANG Yuxuan <stonekyx@gmail.com>
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
