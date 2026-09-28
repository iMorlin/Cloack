#include "mainwindowclient.h"
#include "settings.h"
#include "ui_mainwindowclient.h"

#include "connectionclient.h"
#include "clockwidget.h"

#include <QTime>

MainWindowClient::MainWindowClient(const Settings &settings, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClient)
{
    ui->setupUi(this);
    setWindowTitle("Клиент");

    clock = new ClockWidget(this);
    ui->gridLayout_2->addWidget(clock, 0, 0);
    ui->gridLayout->setRowStretch(0, 1);

    connection = new ConnectionClient(settings, this);
    connect(connection, &ConnectionClient::getTime, this, &MainWindowClient::changeClock);
    connect(connection, &ConnectionClient::getTime, this, &MainWindowClient::changeStatus);
}

MainWindowClient::~MainWindowClient()
{
    delete ui;
}

void MainWindowClient::changeClock(QTime time)
{
    clock->setTime(time);

    if(time.isValid())
        connection->confirmTime();
}

void MainWindowClient::changeStatus(QTime time)
{
    ui->status->setText(time.isValid() ? QStringLiteral("Да") : QStringLiteral("Нет"));
}
