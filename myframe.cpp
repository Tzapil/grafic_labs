#include "myframe.h"

//----------------------------------------CONSTRUCTORS----------------------------------------------------
MyFrame::MyFrame()
{
    x1 = 0; x2 = 0; x3 = 0; x4 = 0;
    y1 = 0; y2 = 0; y3 = 0; y4 = 0;
}

MyFrame::MyFrame(const QRect &rect)
{
    x4 = x1 = rect.x();
    y2 = y1 = rect.y();

    x2 = x3 = x1 + rect.width();
    y3 = y4 = y1 + rect.height();
}

MyFrame::MyFrame(const QPoint &p1, const QPoint &p2, const QPoint &p3, const QPoint &p4)
{
    x1 = p1.x(); y1 = p1.y();
    x2 = p2.x(); y2 = p2.y();
    x3 = p3.x(); y3 = p3.y();
    x4 = p4.x(); y4 = p4.y();
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
}
