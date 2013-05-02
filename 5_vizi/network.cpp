#include <QtNetwork>
#include "network.h"

using namespace std;

const int port = 3141;
int  uptime = 2000;

Network::Network(QWidget *parent) : QWidget(parent) {
    udpSocketReceiver = new QUdpSocket(this);
    udpSocketReceiver->bind(port);
    udpSocketSender = new QUdpSocket(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processPendingDatagrams()));
}

void Network::broadcastDatagram() {
    QByteArray datagram("GET\r\n");
    udpSocketSender->writeDatagram(datagram.data(), datagram.size(),QHostAddress::Broadcast, port);
    timer->start(uptime);
}

int command_length = 9, ent = 2;

void Network::processPendingDatagrams() {
    timer->stop();
    responce.clear();
    while (udpSocketReceiver->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocketReceiver->pendingDatagramSize());
        udpSocketReceiver->readDatagram(datagram.data(), datagram.size());
        string t = "";
        if (datagram.size() != 0 && datagram.at(0) == 'R') {           
            bool flag = false;
            for (int i = command_length; i < datagram.size() - ent; i++) {
                t += datagram.at(i);
            }
            for (size_t i = 0; i < responce.size(); i++) {
                if (t == responce[i])
                    flag = true;
            }
            if (!flag)
                responce.push_back(t);
        }
    }
    cout << responce.size() << " --size" << endl;
    for (size_t i = 0; i < responce.size(); i++) {
        cout << responce[i] << endl;
    }
    cout << "---------" << endl;
    //----------------------------------
    responce.clear();
    responce.push_back("a 1 b 2 c 3 d");
    responce.push_back("b 0 a 2 c 3 d");
    responce.push_back("c 0 a 1 b 3 d");
    responce.push_back("d 0 a 1 b 3 c");
    //----------------------------------
    emit ready_update();
}
