#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H

#include <QPoint>
#include <QImage>
#include <cmath>
#include <tuple>
#include <vector>

#include "atransform.h"

class PerspectiveTransform:public ATransform
{
public:
    PerspectiveTransform();

    void reverse();

    QPoint transformPoint(const QPoint &point);

    void translate(double dx, double dy);
    void scale(double sx = 1, double sy = 1);
    void rotate(double angle);
    void shear(double shx = 0, double shy = 0);

    void generateFrom3Points(std::tuple<QPoint, QPoint, QPoint> in_points, std::tuple<QPoint, QPoint, QPoint> out_points);

//--------------------------------------------MATRIX_GET----------------------------------
    virtual inline double m11() const {return a;}
    virtual inline double m12() const {return b;}
    virtual inline double m13() const {return p;}
    virtual inline double m21() const {return c;}
    virtual inline double m22() const {return d;}
    virtual inline double m23() const {return q;}
    virtual inline double m31() const {return l;}
    virtual inline double m32() const {return m;}
    virtual inline double m33() const {return 1;}
};

#endif // MYTRANSFORM_H
