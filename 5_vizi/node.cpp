#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include "iostream"

Node::Node(GraphWidget *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Node::~Node() {
    std::cout << "destrNode\n";
}

void Node::addEdge(Edge *edge) {
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const {
    return edgeList;
}

QRectF Node::boundingRect() const {
       return QRectF(-r, -r, r + r / 2, r + r / 2);
}

QPainterPath Node::shape() const {
    QPainterPath path;
    path.addEllipse(-r, -r, r + r / 2, r + r / 2);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(-(r / 2), -(r / 2), r, r);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}
