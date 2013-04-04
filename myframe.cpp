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
    points.clear();
    points.resize(4, std::make_pair(0,0));

    init();
}

MyFrame::MyFrame(const QRect &rect)
{
    points.clear();
    int x = rect.x(),
        y = rect.y(),
        w = rect.width(),
        h = rect.height();

    points.push_back(std::make_pair(x, y));
    points.push_back(std::make_pair(x+w, y));
    points.push_back(std::make_pair(x+w, y+h));
    points.push_back(std::make_pair(x, y+h));

    init();
}

MyFrame::MyFrame(const QPoint &p1, const QPoint &p2, const QPoint &p3, const QPoint &p4)
{
    points.clear();
    points.push_back(std::make_pair(p1.x(),p1.y()));
    points.push_back(std::make_pair(p2.x(),p2.y()));
    points.push_back(std::make_pair(p3.x(),p3.y()));
    points.push_back(std::make_pair(p4.x(),p4.y()));

    init();
}

MyFrame::MyFrame(const std::tuple<QPoint, QPoint, QPoint, QPoint> &fr)
{
    points.clear();
    const QPoint *p = &std::get<0>(fr);
    points.push_back(std::make_pair(p->x(),p->y()));
    p = &std::get<1>(fr);
    points.push_back(std::make_pair(p->x(),p->y()));
    p = &std::get<2>(fr);
    points.push_back(std::make_pair(p->x(),p->y()));
    p = &std::get<3>(fr);
    points.push_back(std::make_pair(p->x(),p->y()));

    init();
}

MyFrame::MyFrame(const std::array<QPoint, 4> &v)
{
    points.clear();
    for(auto it=v.begin(); it!=v.end(); it++)
        points.push_back(std::make_pair((*it).x(), (*it).y()));

    init();
}


MyFrame::MyFrame(const std::vector<QPoint> &v)
{
    if(v.size() < 4)
        throw std::range_error("Bad vector");

    points.clear();
    for(auto it=v.begin(); it!=v.end(); it++)
        points.push_back(std::make_pair((*it).x(), (*it).y()));

    init();
}

void MyFrame::translate(int dx, int dy)
{

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
    for_each(points.begin(), points.end(), [&polygon](std::pair<int,int> p){polygon << QPoint(p.first, p.second);});
    painter.drawPolygon(polygon);

    painter.restore();
}

void MyFrame::grubMove(int dx, int dy, uint num)
{
    points.at(num - 1).first += dx;
    points.at(num - 1).second += dy;
    if(points.at(num - 1).first<x_low_bound)
        points.at(num - 1).first = x_low_bound;
    if(points.at(num - 1).first>=x_hi_bound)
        points.at(num - 1).first = x_hi_bound-1;
    if(points.at(num - 1).second<y_low_bound)
        points.at(num - 1).second = y_low_bound;
    if(points.at(num - 1).second>=y_hi_bound)
        points.at(num - 1).second = y_hi_bound-1;
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
    std::for_each(points.begin(), points.end(), [dx, dy](std::pair<int,int> &p){p.first += dx; p.second += dy;});
}

bool MyFrame::captured(int x, int y)
{
    for(int i=0;i<points.size();++i)
    {
        if(points.at(i).first+10>x && points.at(i).first-10<x && points.at(i).second+10>y && points.at(i).second-10<y)
        {
            grub_point = i+1;
            return true;
        }
    }

    QPoint p1(points.at(0).first, points.at(0).second),p2(points.at(1).first, points.at(1).second),
           p3(points.at(2).first, points.at(2).second),p4(points.at(3).first, points.at(3).second);
    auto tr1 = std::make_tuple(p1, p2, p3),
         tr2 = std::make_tuple(p3, p4, p1);
    if(ATransform::pointInTriangle(QPoint(x, y), tr1) || ATransform::pointInTriangle(QPoint(x, y), tr2))
    {
        capture = true;
        return true;
    }

    return false;
}
