#pragma once

#include <QString>
#include <QtGlobal>

class Settings
{
public:
    bool load(const QString &path);

    quint16 port = 0;
    int connectionTimeoutMs = 0;
    int heartbeatIntervalMs = 0;
    int sendTimeIntervalMs = 0;
    int clockStepIntervalMs = 0;
    int clockStepSeconds = 0;
    int clockResumeDelayMs = 0;
};
