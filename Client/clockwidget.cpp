#include "clockwidget.h"

#include <QPainter>

ClockWidget::ClockWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(120, 120);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ClockWidget::setTime(QTime newTime)
{
    if (!newTime.isValid())
        return;

    time = newTime;
    update();
}

QSize ClockWidget::sizeHint() const
{
    return QSize(220, 220);
}

void ClockWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    painter.translate(width() / 2.0, height() / 2.0);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(QPen(Qt::black, 10));
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPointF(0, 0), 90, 90);

    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(QPointF(0, -72), QPointF(0, -80));
        painter.rotate(30);
    }

    painter.save();
    painter.rotate((time.hour() % 12) * 30.0 + time.minute() * 0.5);
    painter.setPen(QPen(Qt::black, 8, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(0, 0), QPointF(0, -42));
    painter.restore();

    painter.save();
    painter.rotate(time.minute() * 6.0);
    painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(0, 0), QPointF(0, -62));
    painter.restore();

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPointF(0, 0), 8, 8);
}
