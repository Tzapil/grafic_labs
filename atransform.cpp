#include "atransform.h"

ATransform::ATransform(const ATransform &c)
{
    clone(c);

    unsave();
}

void ATransform::clone(const ATransform & c)
{
    m_11 = c.m11(); m_12 = c.m12(); m_13 = c.m13();
    m_21 = c.m21(); m_22 = c.m22(); m_23 = c.m23();
    m_31 = c.m31(); m_32 = c.m32(); m_33 = c.m33();
}

bool ATransform::pointInTriangle(const QPoint &point, std::vector<QPoint> triangle)
{
    QPoint *p1 = &(triangle.at(0)),
           *p2 = &(triangle.at(1)),
           *p3 = &(triangle.at(2));

            int pl1, pl2, pl3;
            pl1 = (p1->x() - point.x())*(p2->y() - p1->y())-(p2->x() - p1->x())*(p1->y() - point.y());
            pl2 = (p2->x() - point.x())*(p3->y() - p2->y())-(p3->x() - p2->x())*(p2->y() - point.y());
            pl3 = (p3->x() - point.x())*(p1->y() - p3->y())-(p1->x() - p3->x())*(p3->y() - point.y());
            if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0))
                return true;

            return false;
}

void ATransform::compositeWith(const ATransform &c)
{
    double
        a = m_11, b = m_12,
        o = m_21, d = m_22,
        l = m_31, m = m_32,
        p = m_13, q = m_23,
        g = m_33;
    m_11 = a*c.m11() + b*c.m21() + p*c.m31();
    m_12 = a*c.m12() + b*c.m22() + p*c.m32();
    m_13 = a*c.m13() + b*c.m23() + p*c.m33();

    m_21 = o*c.m11() + d*c.m21() + q*c.m31();
    m_22 = o*c.m12() + d*c.m22() + q*c.m32();
    m_23 = o*c.m13() + d*c.m23() + q*c.m33();

    m_31 = l*c.m11() + m*c.m21() + g*c.m31();
    m_32 = l*c.m12() + m*c.m22() + g*c.m32();
    m_33 = l*c.m13() + m*c.m23() + g*c.m33();
}

QPointF ATransform::transformPoint(const QPointF &point)
{
    int x = point.x(), y = point.y();
    double new_x = m_11*x+m_21*y+m_31,
           new_y = m_12*x+m_22*y+m_32,
           new_w = m_13*x+m_23*y+m_33;

    new_x /= new_w;
    new_y /= new_w;

    return QPoint((int)new_x, (int)new_y);
}

bool ATransform::transformImageDirect(const QImage &in_img, QImage &out_img, const MyFrame &frame)
{
    if(in_img.width()>out_img.width() || in_img.height()>out_img.height())
        return false;

    auto min = frame.getMinXY(),
         max = frame.getMaxXY();
    uint mw = min.first,
         mh = min.second,
         w = max.first,
         h = max.second;

    for(uint y=mh;y<h;++y)
        for(uint x=mw;x<w;++x)
        {
            QRgb color;
            QPoint pp(x, y);
            if(!frame.containsPoint(pp))
                continue;

            QPointF rp = transformPoint(pp);
            int rx = rp.x(), ry = rp.y();
            if(rx<0 || ry<0 || rx>=w || ry>=h)
               continue;
            color = in_img.pixel(x, y);
            out_img.setPixel(rx, ry, color);
        }

    return true;
}

bool ATransform::transformImage(const QImage &in_img, QImage &out_img, const MyFrame &frame)
{
    uint width = in_img.width(),
         height = in_img.height();

    if(width>out_img.width() || height>out_img.height())
        return false;

    auto min = frame.getMinXY(),
         max = frame.getMaxXY();
    uint mw = min.first,
         mh = min.second,
         w = max.first,
         h = max.second;

    for(uint y=mh;y<h;++y)
        for(uint x=mw;x<w;++x)
        {
            QRgb color;
            QPoint pp(x, y);
            if(!frame.containsPoint(pp))
                continue;
            QPointF rp = this->transformPoint(pp);
            int rx = rp.x(), ry = rp.y();
            if(rx<0 || ry<0 || rx>=width || ry>=height)
               continue;
            color = in_img.pixel(rx, ry);
            out_img.setPixel(x, y, color);
        }

    return true;
}

void ATransform::drop()
{
    m_11 = 1; m_12 = 0; m_13 = 0;
    m_21 = 0; m_22 = 1; m_23 = 0;
    m_31 = 0; m_32 = 0; m_33 = 1;
}

void ATransform::unsave()
{
    s_11 = 1; s_12 = 0; s_13 = 0;
    s_21 = 0; s_22 = 1; s_23 = 0;
    s_31 = 0; s_32 = 0; s_33 = 1;
}

void ATransform::save()
{
    s_11 = m_11; s_12 = m_12; s_13 = m_13;
    s_21 = m_21; s_22 = m_22; s_23 = m_23;
    s_31 = m_31; s_32 = m_32; s_33 = m_33;
}

void ATransform::restore()
{
    m_11 = s_11; m_12 = s_12; m_13 = s_13;
    m_21 = s_21; m_22 = s_22; m_23 = s_23;
    m_31 = s_31; m_32 = s_32; m_33 = s_33;
}
