#ifndef BILINETRANSFORM_H
#define BILINETRANSFORM_H

#include <QPoint>
#include <QImage>
#include <cmath>
#include <tuple>
#include <vector>

#include "atransform.h"
#include "agg_simul_eq.h"

class BilineTransform : public ATransform
{
public:
    BilineTransform();

    void reverse();

    QPointF transformPoint(const QPoint &point);

    void compositeWith(const ATransform &c) ;

    void generateFromPoints(std::vector<QPoint> in_points, std::vector<QPoint> out_points);

//--------------------------------------------MATRIX_GET----------------------------------
    virtual inline double m11() const {return m_11;}
    virtual inline double m12() const {return m_12;}
    virtual inline double m13() const {return m_13;}
    virtual inline double m21() const {return m_21;}
    virtual inline double m22() const {return m_22;}
    virtual inline double m23() const {return m_23;}
    virtual inline double m31() const {return m_31;}
    virtual inline double m32() const {return m_32;}
    virtual inline double m33() const {return 1;}
};

#endif // BILINETRANSFORM_H
