#include "heatmap.h"

HeatMap::HeatMap() {}


QImage HeatMap::createPolygonIm(const std::vector<Point>& line_points, int width, int height,
                              const ChartAxes& axes, QColor lineColor, int lineWidth) {

    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);


    if (line_points.size() < 3) {
        qWarning() << "Insufficient points to create a polygon. Returning empty image.";
        return image;
    }


    double scaleX = static_cast<double>(width) / (axes.maxX - axes.minX);
    double scaleY = static_cast<double>(height) / (axes.maxY - axes.minY);


    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(lineColor);
    pen.setWidth(lineWidth);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);


    QPolygonF polygon;
    for (const auto& point : line_points) {
        double x = (point.x - axes.minX) * scaleX;
        double y = (axes.maxY - point.y) * scaleY;
        polygon << QPointF(x, y);
    }  
    painter.drawPolygon(polygon);
    painter.end();

    return image;
}





DOPImages  HeatMap::createHeatmapsImForDOPs(const std::vector<TestingPoints>& tag_points,
                                           const std::vector<Anchor>& anchors,
                                           int width, int height,
                                           const ChartAxes& axes,
                                           int color_value, int z, int method)
{

    DOPImages res;
    QImage image_gdop(width, height, QImage::Format_ARGB32);
    image_gdop.fill(Qt::transparent);
    QImage image_hdop(width, height, QImage::Format_ARGB32);
    image_hdop.fill(Qt::transparent);
    QImage image_vdop(width, height, QImage::Format_ARGB32);
    image_vdop.fill(Qt::transparent);

    double value = 0;
    int blockSize = 15;
    int anchorBlockSize = 5;

    double scaleX = static_cast<double>(width) / (axes.maxX - axes.minX);
    double scaleY = static_cast<double>(height) / (axes.maxY - axes.minY);


    for (int i = 0; i < tag_points.size(); ++i) {
        if(method == 1)
            value = tag_points[i].dopValuesTDoA[z].GDOP;
        else
            value = tag_points[i].dopValuesToA[z].GDOP;
        QColor color;
        if (color_value == -1)
            color = getColorForValue(value);
        else
            color = getColorForValueMax(value, color_value);

        int x = static_cast<int>((tag_points[i].x - axes.minX) * scaleX);
        int y = static_cast<int>((axes.maxY - tag_points[i].y) * scaleY);


        for (int dx = 0; dx < blockSize; ++dx) {
            for (int dy = 0; dy < blockSize; ++dy) {
                int px = x + dx - blockSize / 2;
                int py = y + dy - blockSize / 2;

                if (px >= 0 && px < width && py >= 0 && py < height) {
                    image_gdop.setPixelColor(px, py, color);
                }
            }
        }
    }
    for (int i = 0; i < tag_points.size(); ++i) {
        if(method == 1)
            value = tag_points[i].dopValuesTDoA[z].HDOP;
        else
            value = tag_points[i].dopValuesToA[z].HDOP;
        QColor color;
        if (color_value == -1)
            color = getColorForValue(value);
        else
            color = getColorForValueMax(value, color_value);

        int x = static_cast<int>((tag_points[i].x - axes.minX) * scaleX);
        int y = static_cast<int>((axes.maxY - tag_points[i].y) * scaleY);

        for (int dx = 0; dx < blockSize; ++dx) {
            for (int dy = 0; dy < blockSize; ++dy) {
                int px = x + dx - blockSize / 2;
                int py = y + dy - blockSize / 2;

                if (px >= 0 && px < width && py >= 0 && py < height) {
                    image_hdop.setPixelColor(px, py, color);
                }
            }
        }
    }
    for (int i = 0; i < tag_points.size(); ++i) {
        if(method == 1)
            value = tag_points[i].dopValuesTDoA[z].VDOP;
        else
            value = tag_points[i].dopValuesToA[z].VDOP;
        QColor color;
        if (color_value == -1)
            color = getColorForValue(value);
        else
            color = getColorForValueMax(value, color_value);

        int x = static_cast<int>((tag_points[i].x - axes.minX) * scaleX);
        int y = static_cast<int>((axes.maxY - tag_points[i].y) * scaleY);

        for (int dx = 0; dx < blockSize; ++dx) {
            for (int dy = 0; dy < blockSize; ++dy) {
                int px = x + dx - blockSize / 2;
                int py = y + dy - blockSize / 2;

                if (px >= 0 && px < width && py >= 0 && py < height) {
                    image_vdop.setPixelColor(px, py, color);
                }
            }
        }
    }
    res.GDOP_im = image_gdop;
    res.HDOP_im = image_hdop;
    res.VDOP_im = image_vdop;
    return res;

}


QImage HeatMap::createHeatmapIm(const std::vector<Anchor>& points_calc,
                              const std::vector<double>& dop_values,
                              const std::vector<Anchor>& anchors,
                              int width, int height,
                              const ChartAxes& axes,
                              int color_value)
{
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    int blockSize = 15;
    int anchorBlockSize = 5;

    double scaleX = static_cast<double>(width) / (axes.maxX - axes.minX);
    double scaleY = static_cast<double>(height) / (axes.maxY - axes.minY);

    for (int i = 0; i < points_calc.size(); ++i) {
        double value = dop_values[i];
        QColor color;
        if (color_value == -1)
            color = getColorForValue(value);
        else
            color = getColorForValueMax_anchors(value, color_value);

        int x = static_cast<int>((points_calc[i].x - axes.minX) * scaleX);
        int y = static_cast<int>((axes.maxY - points_calc[i].y) * scaleY);

        for (int dx = 0; dx < blockSize; ++dx) {
            for (int dy = 0; dy < blockSize; ++dy) {
                int px = x + dx - blockSize / 2;
                int py = y + dy - blockSize / 2;

                if (px >= 0 && px < width && py >= 0 && py < height) {
                    image.setPixelColor(px, py, color);
                }
            }
        }
    }

    return image;
}


QImage HeatMap::createAnchorsIm(const std::vector<Anchor>& anchors, int width, int height, const ChartAxes& axes)
{

    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    int anchorRadius = 5;

    double scaleX = static_cast<double>(width) / (axes.maxX - axes.minX);
    double scaleY = static_cast<double>(height) / (axes.maxY - axes.minY);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < anchors.size(); ++i) {
        int x = static_cast<int>((anchors[i].x - axes.minX) * scaleX);
        int y = static_cast<int>((axes.maxY - anchors[i].y) * scaleY);


        QColor anchorColor(0, 0, 0);
        painter.setBrush(anchorColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(x, y), anchorRadius, anchorRadius);

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10));
        QString anchorId = QString::number(anchors[i].ID);


        int textX = x + anchorRadius + 2;
        int textY = y - anchorRadius - 2;
        painter.drawText(textX, textY, anchorId);
    }

    painter.end();
    return image;
}

QImage HeatMap::createVisiblePintsIm(const std::vector<TestingPoints>& tags, int width, int height,
                               const ChartAxes& axes, Anchor anch)
{

    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    int tagRadius = 2;

    double scaleX = static_cast<double>(width) / (axes.maxX - axes.minX);
    double scaleY = static_cast<double>(height) / (axes.maxY - axes.minY);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (const auto& tag : tags) {
        int x = static_cast<int>((tag.x - axes.minX) * scaleX);
        int y = static_cast<int>((axes.maxY - tag.y) * scaleY);

        QColor tagColor(0, 0, 255);
        painter.setBrush(tagColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(x, y), tagRadius, tagRadius);
    }

    int anchorRadius = 5;
    int x = static_cast<int>((anch.x - axes.minX) * scaleX);
    int y = static_cast<int>((axes.maxY - anch.y) * scaleY);
    QColor anchorColor(255, 0, 0);
    painter.setBrush(anchorColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(x, y), anchorRadius, anchorRadius);

    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 10));
    QString anchorId = QString::number(anch.ID);

    int textX = x + anchorRadius + 2;
    int textY = y - anchorRadius - 2;
    painter.drawText(textX, textY, anchorId);
    painter.end();

    return image;
}

QImage HeatMap::createTestingPointsIm(const std::vector<TestingPoints>& tags, int width, int height,
                               const ChartAxes& axes)
{
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    int anchorRadius = 1;

    double scaleX = static_cast<double>(width) / (axes.maxX - axes.minX);
    double scaleY = static_cast<double>(height) / (axes.maxY - axes.minY);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (const auto& tag : tags) {
        int x = static_cast<int>((tag.x - axes.minX) * scaleX);
        int y = static_cast<int>((axes.maxY - tag.y) * scaleY);

        QColor tagColor(0, 0, 0);
        painter.setBrush(tagColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(x, y), anchorRadius, anchorRadius);
    }

    painter.end();

    return image;
}

QColor HeatMap::getColorForValue(double value)
{
    if (value < 0 || value > 8.0)
        return QColor(255, 1, 1);

    double t = value / 8.0;

    double r = 0, g = 0, b = 0;

    if (t < 0.25) {
        b = 255;
        g = 4 * 255 * t;
    } else if (t < 0.5) {
        b = 255 - 4 * 255 * (t - 0.25);
        g = 255;
    } else if (t < 0.75) {
        r = 4 * 255 * (t - 0.5);
        g = 255;
    } else {
        r = 255;
        g = 255 - 4 * 255 * (t - 0.75);
    }

    return QColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
}

QColor HeatMap::getColorForValueMax_anchors(double value, double color_value)
{


    int b = static_cast<int>(255 * qMin(value / color_value, 1.0));
    int r = 255 - b;
    return QColor(r, 0, b);
}
QColor HeatMap::getColorForValueMax(double value, double color_value)
{
    if (color_value <= 0 || value < 0 || value > color_value)
        return QColor(255, 1, 1);

    double t = value / color_value;

    double r = 0, g = 0, b = 0;

    if (t < 0.25) {
        b = 255;
        g = 4 * 255 * t;
    } else if (t < 0.5) {
        b = 255 - 4 * 255 * (t - 0.25);
        g = 255;
    } else if (t < 0.75) {
        r = 4 * 255 * (t - 0.5);
        g = 255;
    } else {
        r = 255;
        g = 255 - 4 * 255 * (t - 0.75);
    }

    return QColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
}



