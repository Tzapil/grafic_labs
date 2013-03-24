#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H

#include <QPoint>
#include <QImage>
#include <cmath>
#include <tuple>
#include <vector>

class MyTransform
{
public:
    MyTransform();

    void reverse();

    QPoint transformPoint(const QPoint &point);
    QImage* transformImage(QImage &in_img);

    void translate(double dx, double dy);
    void scale(double sx = 1, double sy = 1);
    void rotate(double angle);
    void shear(double shx = 0, double shy = 0);

    void generateFrom3Points(std::tuple<QPoint, QPoint, QPoint> in_points, std::tuple<QPoint, QPoint, QPoint> out_points);

    void drop();

    static bool pointInTriangle(const QPoint &point, std::tuple<QPoint, QPoint, QPoint> triangle);
private:
    double a, b, c, d, m, l;
};

#endif // MYTRANSFORM_H
