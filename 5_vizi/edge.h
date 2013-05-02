#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode);
    ~Edge();

    Node *sourceNode() const;
    Node *destNode() const;

    void adjust();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Node *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
};
#endif
