#include "myframe.h"

//----------------------------------------CONSTRUCTORS----------------------------------------------------
MyFrame::MyFrame()
{
    x1 = 0; x2 = 0; x3 = 0; x4 = 0;
    y1 = 0; y2 = 0; y3 = 0; y4 = 0;

    object_count ++;
    number = object_count;
    capture = false;
    grub_point = 0;
}

MyFrame::MyFrame(const QRect &rect)
{
    x4 = x1 = rect.x();
    y2 = y1 = rect.y();

    x2 = x3 = x1 + rect.width();
    y3 = y4 = y1 + rect.height();

    object_count ++;
    number = object_count;
}

MyFrame::MyFrame(const QPoint &p1, const QPoint &p2, const QPoint &p3, const QPoint &p4)
{
    x1 = p1.x(); y1 = p1.y();
    x2 = p2.x(); y2 = p2.y();
    x3 = p3.x(); y3 = p3.y();
    x4 = p4.x(); y4 = p4.y();

    object_count ++;
    number = object_count;
    capture = false;
    grub_point = 0;
}

MyFrame::MyFrame(const std::tuple<QPoint, QPoint, QPoint, QPoint> &fr)
{
    const QPoint *p = &std::get<0>(fr);
    x1 = p->x(); y1 = p->y();
    p = &std::get<1>(fr);
    x2 = p->x(); y2 = p->y();
    p = &std::get<2>(fr);
    x3 = p->x(); y3 = p->y();
    p = &std::get<3>(fr);
    x4 = p->x(); y4 = p->y();

    object_count ++;
    number = object_count;
    capture = false;
    grub_point = 0;
}

MyFrame::MyFrame(const std::array<QPoint, 4> &v)
{
    const QPoint *p = &v.at(0);
    x1 = p->x(); y1 = p->y();
    p = &v.at(1);
    x2 = p->x(); y2 = p->y();
    p = &v.at(2);
    x3 = p->x(); y3 = p->y();
    p = &v.at(3);
    x4 = p->x(); y4 = p->y();

    object_count ++;
    number = object_count;
    capture = false;
    grub_point = 0;
}

void MyFrame::translate(int dx, int dy)
{
    //if(!(x1 <= x2 && x1 <= x3 && x1 <=x4))
        x1 += dx;
    //if(!(x2 <= x1 && x2 <= x3 && x2 <=x4))
        x2 += dx;
    //if(!(x3 <= x2 && x3 <= x1 && x3 <=x4))
        x3 += dx;
    //if(!(x4 <= x2 && x4 <= x3 && x4 <=x1))
        x4 += dx;
    //if(!(y1 <= y2 && y1 <= y3 && y1 <=y4))
        y1 += dy;
    //if(!(y2 <= y1 && y2 <= y3 && y2 <=y4))
        y2 += dy;
    //if(!(y3 <= y2 && y3 <= y1 && y3 <=y4))
        y3 += dy;
    //if(!(y4 <= y2 && y4 <= y3 && y4 <=y1))
        y4 += dy;
}

void MyFrame::release() const
{
    grub_point = 0;
    capture = false;
}

void paint(const QPainter &painter) const
{
    QColor color((number%20)+1);
    color.setAlpha(200 - (capture?100:0));
    QPen pen(color);
    pen.setBrush(QBrush(color));

    painter.save();
    painter.setPen(pen);

    QPolygon polygon;
    polygon << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4);
    painter.drawPolygon(polygon);

    painter.restore();
}

bool MyFrame::captured(uint x, uint y)
{
    auto tr1 = std::make_tuple(QPoint(x1, y1), QPoint(x2, y2), QPoint(x3, y3)),
         tr2 = std::make_tuple(QPoint(x3, y3), QPoint(x4, y4), QPoint(x1, y1));
    if(MyTransform::pointInTriangle(QPoint(x, y), tr1) || MyTransform::pointInTriangle(QPoint(x, y), tr2))
    {
        capture = true;
        return true;
    }

    return false;
}
