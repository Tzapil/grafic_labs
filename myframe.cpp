#include "myframe.h"

uint MyFrame::object_count = 0;
//----------------------------------------INIT-----------------------------------------------

void MyFrame::init()
{
    this->object_count++;
    number = this->object_count;
    capture = false;
    grub_point = 0;
    x_low_bound = 0;
    y_low_bound = 0;
    x_hi_bound = 500;
    y_hi_bound = 500;
}
//----------------------------------------CONSTRUCTORS----------------------------------------------------
MyFrame::MyFrame()
{
    x1 = 0; x2 = 0; x3 = 0; x4 = 0;
    y1 = 0; y2 = 0; y3 = 0; y4 = 0;

    init();
}

MyFrame::MyFrame(const QRect &rect)
{
    x4 = x1 = rect.x();
    y2 = y1 = rect.y();

    x2 = x3 = x1 + rect.width();
    y3 = y4 = y1 + rect.height();

    init();
}

MyFrame::MyFrame(const QPoint &p1, const QPoint &p2, const QPoint &p3, const QPoint &p4)
{
    x1 = p1.x(); y1 = p1.y();
    x2 = p2.x(); y2 = p2.y();
    x3 = p3.x(); y3 = p3.y();
    x4 = p4.x(); y4 = p4.y();

    init();
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

    init();
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

    init();
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

void MyFrame::release()
{
    grub_point = 0;
    capture = false;
}

void MyFrame::paint(QPainter &painter) const
{
    QColor color(255, 0, 0);
    color.setAlpha(100 + (capture || grub_point?50:0));
    QPen pen(color);
    QBrush brush(color);
    //pen.setBrush(QBrush(color));

    painter.save();
    painter.setPen(pen);
    painter.setBrush(brush);

    QPolygon polygon;
    polygon << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4);
    painter.drawPolygon(polygon);

    painter.restore();
}

void MyFrame::grubMove(int dx, int dy, uint num)
{
    switch(num)
    {
        case 1:
            x1 += dx; y1 += dy;
            if(x1<x_low_bound)
                x1 = x_low_bound;
            if(x1>=x_hi_bound)
                x1 = x_hi_bound-1;
            if(y1<y_low_bound)
                y1 = y_low_bound;
            if(y1>=y_hi_bound)
                y1 = y_hi_bound-1;
            break;
        case 2:
            x2 += dx; y2 += dy;
            if(x2<x_low_bound)
                x2 = x_low_bound;
            if(x2>=x_hi_bound)
                x2 = x_hi_bound-1;
            if(y2<y_low_bound)
                y2 = y_low_bound;
            if(y2>=y_hi_bound)
                y2 = y_hi_bound-1;
            break;
        case 3:
            x3 += dx; y3 += dy;
            if(x3<x_low_bound)
                x3 = x_low_bound;
            if(x3>=x_hi_bound)
                x3 = x_hi_bound-1;
            if(y3<y_low_bound)
                y3 = y_low_bound;
            if(y3>=y_hi_bound)
                y3 = y_hi_bound-1;
            break;
        case 4:
            x4 += dx; y4 += dy;
            if(x4<x_low_bound)
                x4 = x_low_bound;
            if(x4>=x_hi_bound)
                x4 = x_hi_bound-1;
            if(y4<y_low_bound)
                y4 = y_low_bound;
            if(y4>=y_hi_bound)
                y4 = y_hi_bound-1;
            break;
    }
}

void MyFrame::dragNdrop(int dx, int dy)
{
    if(grub_point)
        grubMove(dx, dy, grub_point);

    if(capture)
        move(dx, dy);
}

void MyFrame::move(int dx, int dy)
{
    x1 += dx; x2 += dx; x3 += dx; x4 += dx;
    y1 += dy; y2 += dy; y3 += dy; y4 += dy;

    int min_x = MIN4(x1, x2, x3, x4),
        min_y = MIN4(y1, y2, y3, y4),
        max_x = MAX4(x1, x2, x3, x4),
        max_y = MAX4(y1, y2, y3, y4);
    if(min_x<x_low_bound)
    {
        uint mod = abs(min_x);
        x1 += mod; x2 += mod;
        x3 += mod; x4 += mod;
    }
    if(min_y<y_low_bound)
    {
        uint mod = abs(min_y);
        y1 += mod; y2 += mod;
        y3 += mod; y4 += mod;
    }

    if(max_x>=x_hi_bound)
    {
        int mod = x_hi_bound-max_x-1;
        x1 += mod; x2 += mod;
        x3 += mod; x4 += mod;
    }
    if(max_y>=y_hi_bound)
    {
        int mod = y_hi_bound-max_y-1;
        y1 += mod; y2 += mod;
        y3 += mod; y4 += mod;
    }

}

bool MyFrame::captured(int x, int y)
{
    if(x1+10>x && x1-10<x && y1+10>y && y1-10<y)
    {
        grub_point = 1;
        return true;
    }
    if(x2+10>x && x2-10<x && y2+10>y && y2-10<y)
    {
        grub_point = 2;
        return true;
    }
    if(x3+10>x && x3-10<x && y3+10>y && y3-10<y)
    {
        grub_point = 3;
        return true;
    }
    if(x4+10>x && x4-10<x && y4+10>y && y4-10<y)
    {
        grub_point = 4;
        return true;
    }

    auto tr1 = std::make_tuple(QPoint(x1, y1), QPoint(x2, y2), QPoint(x3, y3)),
         tr2 = std::make_tuple(QPoint(x3, y3), QPoint(x4, y4), QPoint(x1, y1));
    if(MyTransform::pointInTriangle(QPoint(x, y), tr1) || MyTransform::pointInTriangle(QPoint(x, y), tr2))
    {
        capture = true;
        return true;
    }

    return false;
}
