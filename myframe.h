#ifndef MYFRAME_H
#define MYFRAME_H

#include <QtGlobal>

#include <QRect>
#include <QPoint>
#include <QPainter>

#include <tuple>
#include <utility>
#include <functional>
#include <array>
#include <vector>
#include <stdexcept>
#include <cmath>

//#include "atransform.h"

#define MIN2(a,b) (a<b?a:b)
#define MIN4(a,b,c,d) (MIN2(MIN2(MIN2(a,b),c),d))
#define MAX2(a,b) (a>b?a:b)
#define MAX4(a,b,c,d) (MAX2(MAX2(MAX2(a,b),c),d))

class MyFrame
{
public:
//--------------------------------------------CONSTRUCTORS---------------------------------
    MyFrame();
    MyFrame(const MyFrame &c);
    MyFrame(const QRect &rect);
    MyFrame(const QPoint &p1, const QPoint &p2, const QPoint &p3, const QPoint &p4);
    MyFrame(const std::tuple<QPoint, QPoint, QPoint, QPoint> &fr);
    MyFrame(const std::array<QPoint, 4> &v);
    MyFrame(const std::vector<QPoint> &v);

//--------------------------------------------PROTECTED_GET--------------------------------
    inline QPoint getPoint(uint pos) const {return QPoint(points.at(pos).first, points.at(pos).second);}      // POINT X1, Y1
    inline int getPointX(uint pos) const {return points.at(pos).first;}
    inline int getPointY(uint pos) const {return points.at(pos).second;}

//--------------------------------------------PROTECTED_SET-------------------------------
    inline void setPoint(const QPoint &p, uint pos){points.at(pos).first = p.x();points.at(pos).second = p.y();}      // POINT X1, Y1
    inline void setPointX(int x, uint pos){points.at(pos).first = x;}
    inline void setPointY(int y, uint pos){points.at(pos).second = y;}

//--------------------------------------------UTILITY-------------------------------------
    inline int getNumber() const {return number;}
    inline void setNumber(int n) {number = n;}

    inline uint pointNumber() const {return points.size();}

    inline void setLowBounds(int x = 0, int y = 0) {x_low_bound = x; x_low_bound = y;}
    inline void setHiBounds(int x = 500, int y = 500) {x_hi_bound = x; y_hi_bound = y;}

    inline bool containsPoint(const QPoint &p) const {
        QPolygon polygon;
        for_each(points.begin(), points.end(), [&polygon](std::pair<int,int> p){polygon << QPoint(p.first, p.second);});
        return polygon.containsPoint(p, Qt::WindingFill);
    }

    inline std::pair<int, int> getMinXY() const {
        int minx = x_hi_bound,
             miny = y_hi_bound;

        std::for_each(points.begin(), points.end(),
        [&minx, &miny](std::pair<int, int> p){
            if(minx>p.first)
                minx = p.first;
            if(miny>p.second)
                miny = p.second;
        });
        return std::make_pair(minx, miny);
    }
    inline std::pair<int, int> getMaxXY() const {
        int maxx = 0,
            maxy = 0;

        std::for_each(points.begin(), points.end(),
        [&maxx, &maxy](std::pair<int, int> p){
            if(maxx<p.first)
                maxx = p.first;
            if(maxy<p.second)
                maxy = p.second;
        });
        return std::make_pair(maxx, maxy);
    }
//--------------------------------------------FUNCTIONAL----------------------------------
    void translate(int dx, int dy);
    bool captured(int x, int y);
    void release();

    void dragNdrop(int dx, int dy);
    void move(int dx, int dy);
    void grubMove(int dx, int dy, uint num);
    void paint(QPainter &painter) const;

protected:
    std::vector<std::pair<int, int> > points;

    uint number;

    bool capture;
    uint grub_point;

    int x_low_bound, y_low_bound,
        x_hi_bound, y_hi_bound;

    static uint object_count;

    void init();
};

#endif // MYFRAME_H
