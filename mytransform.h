#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H

#include <QPoint>
#include <cmath>
#include <tuple>

class MyTransform
{
public:
    MyTransform();

    QPoint transformPoint(const QPoint &point);

    void translate(double dx, double dy);
    void scale(double sx = 1, double sy = 1);
    void rotate(double angle);
    void shear(double shx = 0, double shy = 0);

    void generateFrom3Points(std::tuple<QPoint, QPoint, QPoint> in_points, std::tuple<QPoint, QPoint, QPoint> out_points);

    void drop();
private:
    double a, b, c, d, m, l;
};

#endif // MYTRANSFORM_H
