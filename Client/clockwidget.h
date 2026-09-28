#pragma once

#include <QWidget>
#include <QTime>

class ClockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClockWidget(QWidget *parent = nullptr);

    void setTime(QTime newTime);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTime time = QTime(0, 0);
};
