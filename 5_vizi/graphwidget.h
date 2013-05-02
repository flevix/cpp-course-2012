#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtGui/QGraphicsView>
#include <vector>
#include "mygraph.h"

class Node;
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(myGraph *graph, QWidget *parent = 0);
    ~GraphWidget();

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
};
#endif
