#ifndef ATRANSFORM_H
#define ATRANSFORM_H

#include <QPoint>
#include <QImage>

#include <cmath>
#include <tuple>
#include <vector>

class ATransform
{
public:
    inline ATransform() {}

    inline void reverse() {}

    virtual inline QPoint transformPoint(const QPoint &point){return QPoint(point.x(), point.y());}
    virtual inline QImage* transformImage(QImage &in_img){return new QImage(in_img);}

    virtual inline void translate(double dx, double dy) {}
    virtual inline void scale(double sx = 1, double sy = 1) {}
    virtual inline void rotate(double angle) {}
    virtual inline void shear(double shx = 0, double shy = 0) {}

    virtual inline void generateFrom3Points(std::tuple<QPoint, QPoint, QPoint> in_points, std::tuple<QPoint, QPoint, QPoint> out_points){}

    virtual inline void drop(){}

    static bool pointInTriangle(const QPoint &point, std::tuple<QPoint, QPoint, QPoint> triangle);
};

#endif // ATRANSFORM_H
