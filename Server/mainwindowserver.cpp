#include "mainwindowserver.h"
#include "settings.h"
#include "ui_mainwindowserver.h"

#include "connectionserver.h"

#include <QTimer>

MainWindowServer::MainWindowServer(const Settings &settings, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowServer),
    timerOneMin(new QTimer(this)),
    timerChange(new QTimer(this)),
    clockStepSeconds(settings.clockStepSeconds)
{
    ui->setupUi(this);
    setWindowTitle("Сервер");

    timerOneMin->setInterval(settings.clockStepIntervalMs);
    connect(timerOneMin, &QTimer::timeout, this, &MainWindowServer::addOneMin);

    timerChange->setInterval(settings.clockResumeDelayMs);
    timerChange->setSingleShot(true);
    connect(timerChange, &QTimer::timeout, this, &MainWindowServer::startClock);
    timerChange->start();

    connection = new ConnectionServer(settings, this);
    connection->setTime(ui->timeEdit->time());
    connect(ui->timeEdit, &QTimeEdit::timeChanged, connection, &ConnectionServer::setTime);
    connect(ui->timeEdit, &QTimeEdit::timeChanged, this, &MainWindowServer::pauseClock);
    connect(connection, &ConnectionServer::userGetTime, this, &MainWindowServer::changeStatus);
}

MainWindowServer::~MainWindowServer()
{
    delete ui;
}

void MainWindowServer::changeStatus(bool status)
{
    ui->status->setText(status ? QStringLiteral("Да") : QStringLiteral("Нет"));
}

void MainWindowServer::addOneMin()
{
    changingTime = true;
    ui->timeEdit->setTime(ui->timeEdit->time().addSecs(clockStepSeconds));
    changingTime = false;
}

void MainWindowServer::pauseClock()
{
    if(changingTime)
        return;

    timerOneMin->stop();
    timerChange->start();
}

void MainWindowServer::startClock()
{
    addOneMin();
    timerOneMin->start();
}
