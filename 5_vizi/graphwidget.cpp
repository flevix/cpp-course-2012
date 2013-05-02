#include <QtGui>
#include <math.h>
#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include <iostream>

using namespace std;

const int w = 400, h = 400;

GraphWidget::GraphWidget(myGraph *graph, QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-(w / 2), -(h / 2), w, h);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(w, h);

    int n = (int) graph->matrix.size();
    if (n == 0)
        return;

    vector<Node*> node;

    for (int i = 0; i < n; i++)
    {
        node.push_back(new Node(this));
        QString x = "";
        for (size_t j = 0; j < graph->names[i].size(); j++)
            x += graph->names[i][j];

        node[i]->setToolTip(x);
        scene->addItem(node[i]);
    }

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (graph->matrix[i][j])
                scene->addItem(new Edge(node[i], node[j]));

    double alpha = 0, beta = 2 * M_PI / n;
    double r = (double) w / 4, x = 0, y = 0;

    for (int i = 0; i < n; i++) {
        x = r * cos(alpha);
        y = r * sin(alpha);
        alpha += beta;
        node[i]->setPos(x, y);
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;
    scale(scaleFactor, scaleFactor);
}

void GraphWidget::zoomIn() {
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut() {
    scaleView(1 / qreal(1.2));
}

GraphWidget::~GraphWidget() {
    //scene()->clear()
//    delete scene();
    cout << "destr~~~~~~~~~~~foooooooooooo!!!!!!!!!!!\n";
}
