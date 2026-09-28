#pragma once

#include <QObject>
#include <QTimer>
#include <QHostAddress>
#include <QTime>

class QUdpSocket;
class Settings;
class ConnectionServer : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionServer(const Settings &settings, QObject* parent = nullptr);

    void setTime(QTime time);

signals:
    void clientDetected(const QString &address, quint16 port);
    void userGetTime(bool);

private:
    void readMessages();
    void clientDisconnected();
    void timeoutSendTime();

private:
    QUdpSocket *socket;
    QTimer *timerConnect;
    QTimer *timerSendTime;

    QHostAddress clientAddress;
    quint16 clientPort = 0;
    QTime saveTime = QTime();
};

