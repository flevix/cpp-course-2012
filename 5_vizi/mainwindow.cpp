#include <QtGui>
#include "mainwindow.h"

const int w = 500, h = 500;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMaximumSize(w, h);
    network = new Network(this);

    graph = new myGraph();
    graphWidget = new GraphWidget(graph, this);
    refreshButton = new QPushButton(tr("Start!"));

    connect(refreshButton, SIGNAL(clicked()), this, SLOT(pre_refresh()));
    connect(this, SIGNAL(pre_refresh_fin()), network, SLOT(broadcastDatagram()));
    connect(network, SIGNAL(ready_update()), this, SLOT(refresh()));

    layout = new QVBoxLayout();
    layout->addWidget(refreshButton);
    layout->addWidget(graphWidget);

    box = new QWidget();
    box->setLayout(layout);
    setCentralWidget(box);
}

void MainWindow::pre_refresh() {
    refreshButton->setEnabled(false);
    refreshButton->setText(tr("Updating..."));
    emit pre_refresh_fin();
}

void MainWindow::refresh() {
    refreshButton->setText(tr("Refresh!"));
    refreshButton->setEnabled(true);

    delete graph;
    delete graphWidget;
    graph = new myGraph(network->responce);
    graphWidget = new GraphWidget(graph, this);
    layout->addWidget(graphWidget);

    refreshButton->setEnabled(true);
}

MainWindow::~MainWindow()
{   
    delete graph;
}
