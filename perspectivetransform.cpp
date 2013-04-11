#include "perspectivetransform.h"

PerspectiveTransform::PerspectiveTransform():ATransform()
{
}

QPointF PerspectiveTransform::transformPoint(const QPoint &point)
{
    int x = point.x(), y = point.y();
    double d = 1.0/(x*m_13+y*m_23+1);
    int
        new_x = (m_11*x+m_21*y+m_31)*d,
        new_y = (m_12*x+m_22*y+m_32)*d;

    return QPointF(new_x, new_y);
}

void PerspectiveTransform::compositeWith(const ATransform &c)
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

void PerspectiveTransform::generateFromPoints(std::vector<QPoint> in_points, std::vector<QPoint> out_points)
{
    double left[8][8];
    double right[8][1];
    double m_mtx[8][1];

    unsigned i;
    for (i = 0; i < 4; i++)
    {
        unsigned ix = i * 2;
        unsigned iy = ix + 1;

        QPoint ip = in_points.at(i),
               op = out_points.at(i);

        left[ix][0]  =  1.0;
        left[ix][1]  =  (double)ip.x();
        left[ix][2]  =  (double)ip.y();
        left[ix][3]  =  0.0;
        left[ix][4]  =  0.0;
        left[ix][5]  =  0.0;
        left[ix][6]  = -(double)ip.x() * (double)op.x();
        left[ix][7]  = -(double)ip.y() * (double)op.x();
        right[ix][0] =  (double)op.x();

        left[iy][0]  =  0.0;
        left[iy][1]  =  0.0;
        left[iy][2]  =  0.0;
        left[iy][3]  =  1.0;
        left[iy][4]  =  (double)ip.x();
        left[iy][5]  =  (double)ip.y();
        left[iy][6]  = -(double)ip.x() * (double)op.y();
        left[iy][7]  = -(double)ip.y() * (double)op.y();
        right[iy][0] =  (double)op.y();
    }
    bool m_valid = agg::simul_eq<8, 1>::solve(left, right, m_mtx);

    if(!m_valid)
        return;

    m_31 = m_mtx[0][0];
    m_11 = m_mtx[1][0];
    m_21 = m_mtx[2][0];
    m_32 = m_mtx[3][0];
    m_12 = m_mtx[4][0];
    m_22 = m_mtx[5][0];
    m_13 = m_mtx[6][0];
    m_23 = m_mtx[7][0];
    m_33 = 1;
}

void PerspectiveTransform::reverse()
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
