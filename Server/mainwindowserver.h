#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindowServer;
}

class ConnectionServer;
class QTimer;
class Settings;
class MainWindowServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowServer(const Settings &settings, QWidget *parent = nullptr);
    ~MainWindowServer();

private:
    void changeStatus(bool);
    void addOneMin();
    void pauseClock();
    void startClock();

private:
    Ui::MainWindowServer *ui;
    ConnectionServer *connection;
    QTimer *timerOneMin;
    QTimer *timerChange;
    bool changingTime = false;
    int clockStepSeconds;
};

