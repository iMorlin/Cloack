#pragma once

#include <QObject>
#include <QHostAddress>
#include <QTime>

class QUdpSocket;
class QTimer;
class Settings;
class ConnectionClient : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionClient(const Settings &settings, QObject *parent = nullptr);

    void confirmTime();

signals:
    void getTime(QTime);

private:
    void readMessages();
    void sendHeartbeat();
    void serverDisconnected();

private:
    QUdpSocket *socket;
    QTimer *timerHeartbeat;
    QTimer *timerConnect;
    QHostAddress serverAddress;
    quint16 serverPort;
};
