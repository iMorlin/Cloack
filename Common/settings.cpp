#include "settings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

bool Settings::load(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка открытия настроек:" << path << file.errorString();
        return false;
    }

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError || !document.isObject()) {
        qDebug() << "Ошибка JSON настроек:" << error.errorString();
        return false;
    }

    const QJsonObject object = document.object();
    int serverPort = object.value("port").toInt();
    connectionTimeoutMs = object.value("connectionTimeoutMs").toInt();
    heartbeatIntervalMs = object.value("heartbeatIntervalMs").toInt();
    sendTimeIntervalMs = object.value("sendTimeIntervalMs").toInt();
    clockStepIntervalMs = object.value("clockStepIntervalMs").toInt();
    clockStepSeconds = object.value("clockStepSeconds").toInt();
    clockResumeDelayMs = object.value("clockResumeDelayMs").toInt();

    if (serverPort < 1 || serverPort > 65535 || connectionTimeoutMs <= 0
            || heartbeatIntervalMs <= 0 || sendTimeIntervalMs <= 0
            || clockStepIntervalMs <= 0 || clockResumeDelayMs <= 0 || clockStepSeconds <= 0) {
        qDebug() << "Некорректный порт или интервал в настройках";
        return false;
    }

    if (connectionTimeoutMs <= heartbeatIntervalMs || connectionTimeoutMs <= sendTimeIntervalMs) {
        qDebug() << "Таймаут соединения должен быть больше интервалов обмена";
        return false;
    }

    port = static_cast<quint16>(serverPort);
    return true;
}
