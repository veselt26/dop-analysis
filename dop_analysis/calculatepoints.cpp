#include "calculatepoints.h"
#include "QMessageBox"
#include "polygon.h"

CalculatePoints::CalculatePoints() {}

polygon poly;

bool CalculatePoints::Points_To_Calc(const std::vector<Point>& polygon, std::vector<Anchor>& points_calc, std::vector<TestingPoints>& tag_points, double intv) //vym
{


    if(polygon.size() < 2)
    {
        QMessageBox::warning(nullptr, "Canceled", "U have to draw polygon first.");
        return false;
    }

        double max_x = polygon[0].x;
        double min_x = polygon[0].x;
        double max_y = polygon[0].y;
        double min_y = polygon[0].y;


        for (int i = 1; i < polygon.size(); ++i) {
            if(polygon[i].x > max_x)
            {
                max_x = polygon[i].x;
            }
            if(polygon[i].x < min_x)
            {
                min_x = polygon[i].x;
            }
            if(polygon[i].y > max_y)
            {
                max_y = polygon[i].y;
            }
            if(polygon[i].y < min_y)
            {
                min_y = polygon[i].y;
            }
        }

        Point tag;
        double tmp = min_x;

        while(min_y < max_y)
        {
            min_x = tmp;
            while(min_x < max_x)
            {
                min_x+=intv;
                tag.x = min_x;
                tag.y = min_y;
                tag.z = 0;
                if(poly.insidePol(polygon,tag) == true)
                {
                    Anchor tmp;
                    tmp.x = min_x;
                    tmp.y = min_y;
                    tmp.z = 0;
                    points_calc.push_back(tmp);

                    TestingPoints tmp_x;
                    tmp_x.x = min_x;
                    tmp_x.y = min_y;
                    tmp_x.z = 0;
                    tag_points.push_back(tmp_x);

                }

            }
            min_y+=intv;
        }

    return true;
}
