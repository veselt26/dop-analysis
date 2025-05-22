#include "polygon.h"

polygon::polygon() {}

bool polygon::onSegment(Point p1, Point p2, Point p) {
    return (p.x <= std::max(p1.x, p2.x) && p.x >= std::min(p1.x, p2.x) &&
            p.y <= std::max(p1.y, p2.y) && p.y >= std::min(p1.y, p2.y));
}

int polygon::orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

bool polygon::doIntersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && onSegment(p1, q1, p2)) return true;
    if (o2 == 0 && onSegment(p1, q1, q2)) return true;
    if (o3 == 0 && onSegment(p2, q2, p1)) return true;
    if (o4 == 0 && onSegment(p2, q2, q1)) return true;

    return false;
}

bool polygon::insidePol(const std::vector<Point>& polygon, const Point& p){
    int n = polygon.size();
    if (n < 3) return false;

    Point extreme = {1e9, p.y, 0};
    int count = 0, i = 0;
    do {
        int next = (i + 1) % n;

        if (doIntersect(polygon[i], polygon[next], p, extreme)) {
            if (orientation(polygon[i], polygon[next], p) == 0) {
                return onSegment(polygon[i], polygon[next], p);
            }
            count++;
        }
        i = next;
    } while (i != 0);

    return (count % 2 == 1);
}

int polygon::countVisibleAnchors(const Point& point, const std::vector<Anchor>& anchors, const std::vector<Point>& polygon, std::vector<Anchor>& tmp_a) {
    int visibleCount = 0;

    for (const auto& anchor : anchors) {
        bool visible = true;
        Point tmp;
        tmp.x = anchor.x;
        tmp.y = anchor.y;
        // Pro každou hranu polygonu ověřujeme, zda blokuje viditelnost
        for (size_t i = 0; i < polygon.size(); ++i) {
            Point p1 = polygon[i];
            Point p2 = polygon[(i + 1) % polygon.size()]; // uzavřený polygon
            if (doIntersect(point, tmp, p1, p2)) {
                visible = false;
                break;
            }
        }

        if (visible)
        {
            tmp_a.push_back(anchor);
            ++visibleCount;
        }
    }

    return visibleCount;
}
