#ifndef CALCULATEPOINTS_H
#define CALCULATEPOINTS_H

#include "QVector"
#include "mainwindow.h"


class CalculatePoints
{
public:
    CalculatePoints();
    bool Points_To_Calc(const std::vector<Point>& polygon, std::vector<Anchor>& points_calc, std::vector<TestingPoints>& tag_points, double intv);
};

#endif // CALCULATEPOINTS_H
