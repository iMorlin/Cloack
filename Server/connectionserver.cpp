#include "connectionserver.h"
#include "settings.h"
#include "constants.h"

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDebug>
#include <QTime>

ConnectionServer::ConnectionServer(const Settings &settings, QObject *parent) : QObject(parent),
    socket(new QUdpSocket(this)),
    timerConnect(new QTimer(this)),
    timerSendTime(new QTimer(this))
{
    timerConnect->setInterval(settings.connectionTimeoutMs);
    timerConnect->setSingleShot(true);
    connect(timerConnect, &QTimer::timeout, this, &ConnectionServer::clientDisconnected);

    timerSendTime->setInterval(settings.sendTimeIntervalMs);
    connect(timerSendTime, &QTimer::timeout, this, &ConnectionServer::timeoutSendTime);

    connect(socket, &QUdpSocket::readyRead, this, &ConnectionServer::readMessages);
    if(!socket->bind(QHostAddress::AnyIPv4, settings.port)) {
        qDebug() << "Ошибка bind:" << socket->errorString();
    } else {
        qDebug() << "Ожидание клиента";
    }
}

void ConnectionServer::setTime(QTime time)
{
    saveTime = time;
}

void ConnectionServer::readMessages()
{
    while (socket->hasPendingDatagrams()) {
        const QNetworkDatagram packet = socket->receiveDatagram();
        if(!packet.isValid())
            continue;

        if(packet.data() == HEARTBEAT_MESSAGE) {
            clientAddress = packet.senderAddress();
            clientPort = packet.senderPort();
            timerConnect->start();
            if(!timerSendTime->isActive())
                timerSendTime->start();
            emit userGetTime(true);
        }
        else if(packet.data() == TIME_CONFIRMATION_MESSAGE && clientPort != 0
                && packet.senderAddress() == clientAddress && packet.senderPort() == clientPort) {
            timerConnect->start();
            emit userGetTime(true);
        }
    }
}

void ConnectionServer::clientDisconnected()
{
    timerSendTime->stop();
    clientAddress = QHostAddress();
    clientPort = 0;
    qDebug() << "Клиент не на связи";
    emit userGetTime(false);
}

void ConnectionServer::timeoutSendTime()
{
    if(clientPort == 0)
        return;

    auto data = saveTime.toString(TIME_FORMAT).toUtf8();
    socket->writeDatagram(data, clientAddress, clientPort);
    qDebug() << "Отправили время: " << data;
}

