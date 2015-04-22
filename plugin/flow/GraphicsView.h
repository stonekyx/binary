#ifndef PLUGIN_FLOW_GRAPHICS_VIEW_H
#define PLUGIN_FLOW_GRAPHICS_VIEW_H

#include <QtGui/QGraphicsView>

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
protected:
    virtual void mouseMoveEvent(QMouseEvent *);
signals:
    void mouseScenePos(const QString &);
private:
    QString _mousePosStr;
};

#endif
