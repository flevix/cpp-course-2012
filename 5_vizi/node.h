#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget);
    ~Node();

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    const static int r = 40;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
};
#endif
