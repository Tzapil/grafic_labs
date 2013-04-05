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
    inline ATransform() {drop(); unsave();}
    ATransform(const ATransform &c);

    virtual void reverse() = 0;

    virtual void clone(const ATransform & c);
    virtual void save();
    virtual void unsave();
    virtual void restore();
    virtual void drop();

    virtual QImage* transformImage(QImage &in_img);
    virtual QPoint transformPoint(const QPoint &point);

    virtual void translate(double dx, double dy) = 0;
    virtual void scale(double sx = 1, double sy = 1) = 0;
    virtual void rotate(double angle) = 0;
    virtual void shear(double shx = 0, double shy = 0) = 0;

    virtual void generateFrom3Points(std::tuple<QPoint, QPoint, QPoint> in_points, std::tuple<QPoint, QPoint, QPoint> out_points) = 0;

    virtual inline double m11() const {return m_11;}
    virtual inline double m12() const {return m_12;}
    virtual inline double m13() const {return m_13;}
    virtual inline double m21() const {return m_21;}
    virtual inline double m22() const {return m_22;}
    virtual inline double m23() const {return m_23;}
    virtual inline double m31() const {return m_31;}
    virtual inline double m32() const {return m_32;}
    virtual inline double m33() const {return m_33;}

    static bool pointInTriangle(const QPoint &point, std::tuple<QPoint, QPoint, QPoint> triangle);

protected:
    double m_11, m_12, m_13, m_21, m_22, m_23, m_31, m_32, m_33,
           s_11, s_12, s_13, s_21, s_22, s_23, s_31, s_32, s_33;
};

#endif // ATRANSFORM_H
