#ifndef POLYGON_H
#define POLYGON_H

#include <QVector>
#include "mainwindow.h"



class polygon
{
public:
    polygon();
    bool insidePol(const std::vector<Point>& polygon, const Point& p);
    bool doIntersect(Point p1, Point q1, Point p2, Point q2);
    bool onSegment(Point p1, Point p2, Point p);
    int orientation(Point p, Point q, Point r);
    int countVisibleAnchors(const Point& point, const std::vector<Anchor>& anchors, const std::vector<Point>& polygon, std::vector<Anchor>& tmp_a);

};

#endif // POLYGON_H
