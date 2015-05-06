/*
 * Copyright 2015 KANG Yuxuan
 *
 * FlowDrawer.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <elf.h>
#include <gvc.h>

#include <QtGui/QGraphicsTextItem>
#include <QtGui/QFontMetrics>

#include "FlowDrawer.h"

using namespace std;
BEGIN_PLUG_NAMESPACE(flow)

FlowDrawer::FlowDrawer(GVGraph::GVAlgoType algo, QPaintDevice *pd) :
    _pd(pd),
    _algo(algo)
{
}

void FlowDrawer::draw(QGraphicsScene *scene, const string &src)
{
    QFont font("Courier", 14.0);
    GVGraph graph(_algo, "G", font, _pd);
    QString qsrc = QString::fromStdString(src);
    graph.parseDot(qsrc);
    graph.applyLayout();
    QString minName, maxName;
    Elf64_Addr minAddr=0x7fffffffffffffffll, maxAddr=0;
    QList<GVNode> nodes = graph.nodes();
    foreach(GVNode node, nodes) {
        bool ok;
        Elf64_Addr addr = node.comment.toULongLong(&ok);
        if(!ok) continue;
        if(minAddr > addr) {
            minAddr = addr; minName = node.name;
        }
        if(maxAddr < addr) {
            maxAddr = addr; maxName = node.name;
        }
    }
    QMap<QString, QGraphicsItem*> nodeMap;
    foreach(GVNode node, nodes) {
        QPointF topleft(node.centerPos);
        topleft.setX(topleft.x() - node.width/2.0);
        topleft.setY(topleft.y() - node.height/2.0);
        QPen pen;
        if(minName == node.name) {
            pen.setColor(Qt::red);
        } else if(maxName == node.name) {
            pen.setColor(Qt::green);
        } else {
            pen.setColor(Qt::black);
        }
        nodeMap[node.name] = scene->addRect(topleft.x(), topleft.y(),
                node.width, node.height, pen);
        nodeMap[node.name]->setData(ITEMKEY_NB, QVariant(QList<QVariant>()));
        nodeMap[node.name]->setData(ITEMKEY_COLOR, pen.color());
        QGraphicsItem *txtItem =
            scene->addText(node.label.replace("\\l", "\n"), font);
        txtItem->setPos(topleft);
        QVariant nodeVariant;
        nodeVariant.setValue(nodeMap[node.name]);
        txtItem->setData(ITEMKEY_RECT, nodeVariant);
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

        if(edge.visible) {
            QList<QVariant> neighbor =
                nodeMap[edge.source]->data(ITEMKEY_NB).toList();
            QVariant next;
            next.setValue(nodeMap[edge.target]);
            neighbor << next;
            nodeMap[edge.source]->setData(ITEMKEY_NB, QVariant(neighbor));
        }
    }
}

END_PLUG_NAMESPACE
