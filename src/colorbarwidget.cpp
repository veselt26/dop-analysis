#include "colorbarwidget.h"
#include <QPainter>
#include <QLinearGradient>
#include "heatmap.h"

ColorBarWidget::ColorBarWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumWidth(60);
    setMinimumHeight(80);
}

void ColorBarWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = this->rect();


    QImage gradientImage(1, rect.height(), QImage::Format_RGB32);
    HeatMap h;

    for (int y = 0; y < rect.height(); ++y)
    {
        double t = static_cast<double>(y) / rect.height();
        double value = (1.0 - t) * 8.0;

        QColor color = h.getColorForValue(value);
        gradientImage.setPixelColor(0, y, color);
    }
    painter.drawImage(rect.adjusted(30, 0, 0, 0), gradientImage.scaled(rect.width() - 30, rect.height()));

    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int steps = 8;
    for (int i = 0; i <= steps; ++i)
    {
        int y = rect.top() + (i * rect.height()) / steps;

        if(i != 0 && i != 8)
        {
            painter.drawText(5, y + 5, QString::number(steps - i));
            painter.drawText(5, y + 5, QString::number(steps - i));
            painter.drawLine(25, y, 30, y);

        }
    }
}

