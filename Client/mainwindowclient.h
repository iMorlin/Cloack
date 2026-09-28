#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>

namespace Ui {
class MainWindowClient;
}

class ConnectionClient;
class ClockWidget;
class Settings;
class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowClient(const Settings &settings, QWidget *parent = nullptr);
    ~MainWindowClient();

private:
    void changeClock(QTime);
    void changeStatus(QTime);

private:
    Ui::MainWindowClient *ui;
    ConnectionClient *connection;
    ClockWidget *clock;
};

#endif // MAINWINDOWCLIENT_H
