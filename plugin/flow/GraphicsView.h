/*
 * Copyright 2015 KANG Yuxuan
 *
 * GraphicsView.h by KANG Yuxuan <stonekyx@gmail.com>
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

#ifndef PLUGIN_FLOW_GRAPHICS_VIEW_H
#define PLUGIN_FLOW_GRAPHICS_VIEW_H

#include <QtGui/QGraphicsView>
#include <QtCore/QStack>
#include <QtCore/QMap>
#include <QtCore/QSet>

#include "common.h"

BEGIN_PLUG_NAMESPACE(flow)
namespace Ui{

class GraphicsView;

}
END_PLUG_NAMESPACE

class binary::plugin::flow::Ui::GraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = NULL);
    virtual ~GraphicsView();
    void setScene(QGraphicsScene *);
protected:
    virtual void mouseMoveEvent(QMouseEvent *);
signals:
    void mouseScenePos(const QString &);
private:
    QString _mousePosStr;
    QMap<QGraphicsItem*, int> _scc;
    QStack<QGraphicsItem*> _stack;
    QSet<QGraphicsItem*> _onStack;
    QMap<QGraphicsItem*, int> _index;
    QMap<QGraphicsItem*, int> _lowlink;
    int _maxIndex;
    int _maxScc;
    int _currScc;

    void resetHighlight();
    void highlightLoop(QPoint);
    void highlightScc(int);
    void tarjanInit();
    void tarjan(QGraphicsItem*);
};

#endif
