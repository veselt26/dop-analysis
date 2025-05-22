#ifndef HEATMAP_H
#define HEATMAP_H

#include <QImage>
#include <QColor>
#include "mainwindow.h"


class HeatMap
{
public:
    HeatMap();
    QImage createPolygonIm(const std::vector<Point>& line_points, int width, int height,
                         const ChartAxes& axes, QColor lineColor, int lineWidth);
    QImage createHeatmapIm(const std::vector<Anchor>& points_calc,
                                      const std::vector<double>& dop_values,
                                      const std::vector<Anchor>& anchors,
                                      int width, int height,
                                      const ChartAxes& axes,
                                      int color_value);
    DOPImages createHeatmapsImForDOPs(const std::vector<TestingPoints>& tag_points,
                                  const std::vector<Anchor>& anchors,
                                  int width, int height,
                                  const ChartAxes& axes,
                                  int color_value, int z, int method);
    QImage createAnchorsIm(const std::vector<Anchor>& anchors, int width, int height, const ChartAxes& axes);
    QImage createTestingPointsIm(const std::vector<TestingPoints>& tags, int width, int height,
                          const ChartAxes& axes);
    QImage createVisiblePintsIm(const std::vector<TestingPoints>& tags, int width, int height,
                                  const ChartAxes& axes, Anchor anch);
     QColor getColorForValue(double value);

private:

    QColor getColorForValueMax(double value, double color_value);
    QColor getColorForValueMax_anchors(double value, double color_value);

};



#endif // HEATMAP_H
