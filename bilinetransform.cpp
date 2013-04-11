#include "bilinetransform.h"

BilineTransform::BilineTransform():ATransform()
{
}

QPointF BilineTransform::transformPoint(const QPoint &point)
{
    double x = point.x(), y = point.y(),
           xy = x*y,
           new_x = (m_11*x+m_21*y+m_31+m_13*xy),
           new_y = (m_12*x+m_22*y+m_32+m_23*xy);

    return QPointF(new_x, new_y);
}

void BilineTransform::compositeWith(const ATransform &c)
{
    ATransform::compositeWith(c);

    m_11 /= m_33;
    m_12 /= m_33;
    m_13 /= m_33;

    m_21 /= m_33;
    m_22 /= m_33;
    m_23 /= m_33;

    m_31 /= m_33;
    m_32 /= m_33;
    m_33 = 1;
}

void BilineTransform::generateFromPoints(std::vector<QPoint> in_points, std::vector<QPoint> out_points)
{
    double left[4][4];
    double right[4][2];
    double m_mtx[4][2];

    unsigned i;
    for(i = 0; i < 4; i++)
    {
        unsigned ix = i * 2;
        unsigned iy = ix + 1;


        QPoint ip = in_points.at(i),
               op = out_points.at(i);

        left[i][0] = 1.0;
        left[i][1] = ip.x() * ip.y();
        left[i][2] = ip.x();
        left[i][3] = ip.y();

        right[i][0] = op.x();
        right[i][1] = op.y();
    }
    bool m_valid = agg::simul_eq<4, 2>::solve(left, right, m_mtx);

    if(!m_valid)
        return;

    m_31 = m_mtx[0][0];
    m_11 = m_mtx[2][0];
    m_21 = m_mtx[3][0];
    m_32 = m_mtx[0][1];
    m_12 = m_mtx[2][1];
    m_22 = m_mtx[3][1];
    m_13 = m_mtx[1][0];
    m_23 = m_mtx[1][1];
    m_33 = 1;
}

void BilineTransform::reverse()
{
    double _a = m_11, _b = m_12, _c = m_21, _d = m_22, _l = m_31, _m = m_32,
           _p = m_13, _q = m_23;

    m_33 = _a*_d - _b*_c;
    m_11 = (_a*1 - _q*_m)/m_33;
    m_12 = (_m*_p - _b*1)/m_33;
    m_13 = (_b*_q - _p*_d)/m_33;
    m_21 = (_m*_l - 1*_c)/m_33;
    m_22 = (_a*1 - _p*_l)/m_33;
    m_23 = (_p*_c - _p*_l)/m_33;
    m_31 = (_c*_l - _l*_d)/m_33;
    m_32 = (_l*_b - _a*_m)/m_33;
    m_33 = 1;
}
