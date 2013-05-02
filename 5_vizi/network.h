#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>
#include <vector>
#include <string>
#include <iostream>

QT_BEGIN_NAMESPACE
class QUdpSocket;
class QTimer;
QT_END_NAMESPACE

class Network : public QWidget
{
    Q_OBJECT
public:
    Network(QWidget *parent = 0);
    std::vector<std::string> responce;
private slots:
    void processPendingDatagrams();
    void broadcastDatagram();
signals:
    void ready_update();
private:
    QUdpSocket *udpSocketSender;
    QUdpSocket *udpSocketReceiver;
    QTimer *timer;
};

#endif // NETWORK_H
