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

bool ATransform::pointInTriangle(const QPoint &point, std::tuple<QPoint, QPoint, QPoint> triangle)
{
    QPoint *p1 = &(std::get<0>(triangle)),
           *p2 = &(std::get<1>(triangle)),
           *p3 = &(std::get<2>(triangle));

            int pl1, pl2, pl3;
            pl1 = (p1->x() - point.x())*(p2->y() - p1->y())-(p2->x() - p1->x())*(p1->y() - point.y());
            pl2 = (p2->x() - point.x())*(p3->y() - p2->y())-(p3->x() - p2->x())*(p2->y() - point.y());
            pl3 = (p3->x() - point.x())*(p1->y() - p3->y())-(p1->x() - p3->x())*(p3->y() - point.y());
            if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0))
                return true;

            return false;
}

QPoint ATransform::transformPoint(const QPoint &point)
{
    int x = point.x(), y = point.y();
    double new_x = m_11*x+m_21*y+m_31,
           new_y = m_12*x+m_22*y+m_32,
           new_w = m_13*x+m_23*y+m_33;

    new_x /= new_w;
    new_y /= new_w;

    return QPoint((int)new_x, (int)new_y);
}

QImage* ATransform::transformImage(QImage &in_img)
{
    QImage *new_img = new QImage(in_img.width(),in_img.height(),QImage::Format_RGB32);

    uint w = new_img->width(),
         h = new_img->height();

    for(uint y=0;y<h;++y)
        for(uint x=0;x<w;++x)
        {
            QRgb color;
            QPoint pp(x, y);

            QPoint rp = transformPoint(pp);
            int rx = rp.x(), ry = rp.y();
            if(rx<0 || ry<0 || rx>=w || ry>=h)
               continue;
            color = in_img.pixel(x, y);
            new_img->setPixel(rx, ry, color);
        }

    return new_img;
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
