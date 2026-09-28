#include "connectionclient.h"
#include "settings.h"
#include "constants.h"

#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QNetworkDatagram>

ConnectionClient::ConnectionClient(const Settings &settings, QObject *parent) : QObject(parent),
    socket(new QUdpSocket(this)),
    timerHeartbeat(new QTimer(this)),
    timerConnect(new QTimer(this)),
    serverPort(settings.port)
{
    connect(socket, &QUdpSocket::readyRead, this, &ConnectionClient::readMessages);

    timerConnect->setInterval(settings.connectionTimeoutMs);
    timerConnect->setSingleShot(true);
    connect(timerConnect, &QTimer::timeout, this, &ConnectionClient::serverDisconnected);

    timerHeartbeat->setInterval(settings.heartbeatIntervalMs);
    connect(timerHeartbeat, &QTimer::timeout, this, &ConnectionClient::sendHeartbeat);
    sendHeartbeat();
    timerHeartbeat->start();
}

void ConnectionClient::readMessages()
{
    while (socket->hasPendingDatagrams()) {
        const QNetworkDatagram packet = socket->receiveDatagram();
        if(!packet.isValid() || packet.senderPort() != serverPort)
            continue;

        if(!serverAddress.isNull() && packet.senderAddress() != serverAddress)
            continue;

        if(!packet.data().isEmpty()) {
            QTime time = QTime::fromString(QString::fromUtf8(packet.data()), TIME_FORMAT);
            if (!time.isValid())
                continue;

            serverAddress = packet.senderAddress();
            timerConnect->start();
            qDebug() << "получил время" << time.toString(TIME_FORMAT);
            emit getTime(time);
        }
    }
}

void ConnectionClient::confirmTime()
{
    if(socket->writeDatagram(TIME_CONFIRMATION_MESSAGE, serverAddress, serverPort) == -1)
        qDebug() << "Ошибка подтверждения:" << socket->errorString();
}

void ConnectionClient::sendHeartbeat()
{    
    QHostAddress address = serverAddress;
    if(address.isNull())
        address = SERVER_DISCOVERY_ADDRESS;

    if(socket->writeDatagram(HEARTBEAT_MESSAGE, address, serverPort) == -1)
        qDebug() << "Ошибка отправки Hi:" << socket->errorString();

    qDebug() << "Дал о себе знать" ;
}

void ConnectionClient::serverDisconnected()
{
    serverAddress = QHostAddress();
    qDebug() << "сервер дисконект";
    emit getTime(QTime());
}
