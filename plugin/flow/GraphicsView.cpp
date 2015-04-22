#include <QtGui/QMouseEvent>

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

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QGraphicsView::mouseMoveEvent(e);
    QPointF scenePos = mapToScene(e->pos());
    _mousePosStr = QString::number(scenePos.x()) + ", " +
        QString::number(scenePos.y());
    emit mouseScenePos(_mousePosStr);
}

}
END_PLUG_NAMESPACE
