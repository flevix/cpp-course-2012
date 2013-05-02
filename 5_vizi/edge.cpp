#include <QPainter>

#include "edge.h"
#include "node.h"

#include <math.h>
#include <iostream>

Edge::Edge(Node *sourceNode, Node *destNode) {
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
}

Edge::~Edge() {
    std::cout << "destrEdge\n";
}

Node *Edge::sourceNode() const {
    return source;
}

Node *Edge::destNode() const {
    return dest;
}

void Edge::adjust() {
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * (Node::r / 2)) / length, (line.dy() * (Node::r / 2)) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const {
    if (!source || !dest)
        return QRectF();

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y())).normalized();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    painter->setPen(Qt::blue);
    painter->drawLine(line);
}
