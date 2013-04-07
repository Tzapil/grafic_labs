#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H

#include <QPoint>
#include <QImage>
#include <cmath>
#include <tuple>
#include <vector>

#include "atransform.h"

class AffineTransform:public ATransform
{
public:
    AffineTransform();

    void reverse();

    virtual QPointF transformPoint(const QPointF &point);

    void translate(double dx, double dy);
    void scale(double sx = 1, double sy = 1);
    void rotate(double angle);
    void shear(double shx = 0, double shy = 0);

    virtual void generateFromPoints(std::vector<QPoint> in_points, std::vector<QPoint> out_points);

//--------------------------------------------MATRIX_GET----------------------------------
    virtual inline double m11() const {return m_11;}
    virtual inline double m12() const {return m_12;}
    virtual inline double m13() const {return 0;}
    virtual inline double m21() const {return m_21;}
    virtual inline double m22() const {return m_22;}
    virtual inline double m23() const {return 0;}
    virtual inline double m31() const {return m_31;}
    virtual inline double m32() const {return m_32;}
    virtual inline double m33() const {return 1;}
};

#endif // MYTRANSFORM_H
