#include "affinetransform.h"

AffineTransform::AffineTransform():ATransform()
{
}

void AffineTransform::translate(double dx, double dy)
{
    m_31 += dx;
    m_32 += dy;
}

void AffineTransform::scale(double sx, double sy)
{
    m_11 *= sx; m_21 *= sx; m_31 *= sx;
    m_12 *= sy; m_22 *= sy; m_32 *= sy;
}

void AffineTransform::rotate(double angle)
{
    double rad = angle*M_PI/180,
           sn = sin(rad),
           cs = cos(rad),
           a = m_11, b = m_12,
           c = m_21, d = m_22,
           l = m_31, m = m_32;

    m_11 = cs*a - sn*b; m_12 = sn*a + cs*b;
    m_21 = cs*c - sn*d; m_22 = sn*c + cs*d;
    m_31 = cs*l - sn*m; m_32 = sn*l + cs*m;
}

void AffineTransform::shear(double shx, double shy)
{
    double mult = (shx*shy+1),
           a = m_11, b = m_12,
           c = m_21, d = m_22,
           l = m_31, m = m_32;

    m_11 += b*shx; m_12 = a*shy + b*mult;
    m_21 += d*shx; m_22 = c*shy + d*mult;
    m_31 += m*shx; m_32 = l*shy + m*mult;
}

QPointF AffineTransform::transformPoint(const QPointF &point)
{

    double x = point.x(), y = point.y(),
           new_x = m_11*x+m_21*y+m_31,
           new_y = m_12*x+m_22*y+m_32;

    return QPointF(new_x, new_y);
}

void AffineTransform::generateFromPoints(std::vector<QPoint> in_points, std::vector<QPoint> out_points)
{
    QPoint *ip1 = &(in_points.at(0)),
           *ip2 = &(in_points.at(1)),
           *ip3 = &(in_points.at(2)),

           *op1 = &(out_points.at(0)),
           *op2 = &(out_points.at(1)),
           *op3 = &(out_points.at(2));

   long x_0 = ip1->x(), x_1 = ip2->x(), x_2 = ip3->x(),
        y_0 = ip1->y(), y_1 = ip2->y(), y_2 = ip3->y(),

        ox_0 = op1->x(), ox_1 = op2->x(), ox_2 = op3->x(),
        oy_0 = op1->y(), oy_1 = op2->y(), oy_2 = op3->y(),

        dy12 = y_1-y_2, dy20 = y_2-y_0, dy01 = y_0-y_1,
        dx21 = x_2-x_1, dx02 = x_0-x_2, dx10 = x_1-x_0,

        dxy12 = x_1*y_2-x_2*y_1, dxy20 = x_2*y_0-x_0*y_2, dxy01 = x_0*y_1-x_1*y_0;


    double det = 1.0/(x_0*dy12-y_0*(x_1-x_2)+dxy12);

    m_11 = (dy12*ox_0 + dy20*ox_1 + dy01*ox_2)*det;
    m_12 = (dy12*oy_0 + dy20*oy_1 + dy01*oy_2)*det;
    m_21 = (dx21*ox_0 + dx02*ox_1 + dx10*ox_2)*det;
    m_22 = (dx21*oy_0 + dx02*oy_1 + dx10*oy_2)*det;

    m_31 = (dxy12*ox_0 + dxy20*ox_1 + dxy01*ox_2)*det;
    m_32 = (dxy12*oy_0 + dxy20*oy_1 + dxy01*oy_2)*det;
}

void AffineTransform::reverse()
{
    double _a = m_11, _b = m_12, _c = m_21, _d = m_22, _l = m_31, _m = m_32,
           det = 1.0/(_a*_d-_b*_c);

    m_11 = _d*det; m_12 = -_b*det;
    m_21 = -_c*det; m_22 = _a*det;
    m_31 = det*(_c*_m-_l*_d); m_32 = det*(_l*_b-_a*_m);
}
