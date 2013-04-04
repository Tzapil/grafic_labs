#include "atransform.h"

bool ATransform::pointInTriangle(const QPoint &point, std::tuple<QPoint, QPoint, QPoint> triangle)
{
    QPoint *p1 = &(std::get<0>(triangle)),
           *p2 = &(std::get<1>(triangle)),
           *p3 = &(std::get<2>(triangle));

            int pl1, pl2, pl3;
            pl1 = (p1->x() - point.x())*(p2->y() - p1->y())-(p2->x() - p1->x())*(p1->y() - point.y());
            pl2 = (p2->x() - point.x())*(p3->y() - p2->y())-(p3->x() - p2->x())*(p2->y() - point.y());
            pl3 = (p3->x() - point.x())*(p1->y() - p3->y())-(p1->x() - p3->x())*(p3->y() - point.y());
            if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0))
                return true;

            return false;
}
