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
