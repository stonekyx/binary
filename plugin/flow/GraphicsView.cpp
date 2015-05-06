/*
 * Copyright 2015 KANG Yuxuan
 *
 * GraphicsView.cpp by KANG Yuxuan <stonekyx@gmail.com>
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

#include <QtGui/QMouseEvent>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsRectItem>

#include "FlowDrawer.h"
#include "GraphicsView.h"

BEGIN_PLUG_NAMESPACE(flow)
namespace Ui{

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
}

GraphicsView::~GraphicsView()
{
}

void GraphicsView::setScene(QGraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
    tarjanInit();
}

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QGraphicsView::mouseMoveEvent(e);
    QPointF scenePos = mapToScene(e->pos());
    _mousePosStr = QString::number(scenePos.x()) + ", " +
        QString::number(scenePos.y());
    emit mouseScenePos(_mousePosStr);

    highlightLoop(e->pos());
}

void GraphicsView::resetHighlight()
{
    for(QMap<QGraphicsItem*, int>::const_iterator it = _scc.begin();
            it != _scc.end(); it++)
    {
        if(it.value() == _currScc) {
            QGraphicsItem *key = it.key();
            qgraphicsitem_cast<QGraphicsRectItem*>(key)->setPen(QPen(
                        key->data(FlowDrawer::ITEMKEY_COLOR)
                        .value<QColor>()));
        }
    }
}

void GraphicsView::highlightLoop(QPoint mousePos)
{
    resetHighlight();
    QGraphicsItem *item = itemAt(mousePos);
    if(!qgraphicsitem_cast<QGraphicsRectItem*>(item)) {
        QGraphicsItem *rect = item->data(FlowDrawer::ITEMKEY_RECT)
            .value<QGraphicsItem*>();
        if(qgraphicsitem_cast<QGraphicsRectItem*>(rect)) {
            item = rect;
        } else {
            return;
        }
    }
    if(_scc.find(item) != _scc.end()) {
        highlightScc(_currScc = _scc[item]);
        return;
    }
    _stack.clear();
    _onStack.clear();
    tarjan(item);
    highlightScc(_currScc = _scc[item]);
}

void GraphicsView::highlightScc(int scc)
{
    for(QMap<QGraphicsItem*, int>::const_iterator it = _scc.begin();
            it != _scc.end(); it++)
    {
        if(it.value() == scc) {
            QGraphicsItem *key = it.key();
            qgraphicsitem_cast<QGraphicsRectItem*>(key)->setPen(QPen(Qt::gray));
        }
    }
    _currScc = scc;
}

void GraphicsView::tarjanInit()
{
    _scc.clear();
    _index.clear();
    _lowlink.clear();
    _maxIndex = 0;
    _maxScc = 0;
}

void GraphicsView::tarjan(QGraphicsItem *cur)
{
    _index[cur] = _maxIndex;
    _lowlink[cur] = _maxIndex;
    _maxIndex ++;
    _stack.push(cur);
    _onStack.insert(cur);
    foreach(QVariant var, cur->data(FlowDrawer::ITEMKEY_NB).toList()) {
        QGraphicsItem *next = var.value<QGraphicsItem*>();
        if(_index.find(next) == _index.end()) {
            tarjan(next);
            if(_lowlink[cur] > _lowlink[next]) {
                _lowlink[cur] = _lowlink[next];
            }
        } else if(_onStack.find(next) != _onStack.end()) {
            if(_lowlink[cur] > _index[next]) {
                _lowlink[cur] = _index[next];
            }
        }
    }

    if(_lowlink[cur] == _index[cur]) {
        QGraphicsItem *top;
        do {
            top = _stack.top();
            _scc[top] = _maxScc;
            _onStack.remove(top);
            _stack.pop();
        } while(top != cur);
        _maxScc ++;
    }
}

}
END_PLUG_NAMESPACE
