#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include "mygraph.h"
#include "network.h"
#include "graphwidget.h"
#include <vector>
#include <string>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void refresh();
    void pre_refresh();
signals:
    void pre_refresh_fin();

private:
    QVBoxLayout *layout;
    QWidget *box;
    myGraph *graph;
    GraphWidget *graphWidget;
    QPushButton *refreshButton;
    Network *network;
};

#endif // MAINWINDOW_H
