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
#include <stdexcept>
#include <cmath>

#include "mytransform.h"

#define MIN2(a,b) (a<b?a:b)
#define MIN4(a,b,c,d) (MIN2(MIN2(MIN2(a,b),c),d))
#define MAX2(a,b) (a>b?a:b)
#define MAX4(a,b,c,d) (MAX2(MAX2(MAX2(a,b),c),d))

class MyFrame
{
public:
//--------------------------------------------CONSTRUCTORS---------------------------------
    MyFrame();
    MyFrame(const QRect &rect);
    MyFrame(const QPoint &p1, const QPoint &p2, const QPoint &p3, const QPoint &p4);
    MyFrame(const std::tuple<QPoint, QPoint, QPoint, QPoint> &fr);
    MyFrame(const std::array<QPoint, 4> &v);

//--------------------------------------------PROTECTED_GET--------------------------------
    inline QPoint getP1() const {return QPoint(x1 ,y1);}             // POINT X1, Y1
    inline int getP1X() const {return x1;}
    inline int getP1Y() const {return y1;}

    inline QPoint getP2() const {return QPoint(x2 ,y2);}             // POINT X2, Y2
    inline int getP2X() const {return x2;}
    inline int getP12Y() const {return y2;}

    inline QPoint getP3() const {return QPoint(x3 ,y3);}             // POINT X3, Y3
    inline int getP3X() const {return x3;}
    inline int getP3Y() const {return y3;}

    inline QPoint getP4() const {return QPoint(x4 ,y4);}             // POINT X4, Y4
    inline int getP4X() const {return x4;}
    inline int getP4Y() const {return y4;}

//--------------------------------------------PROTECTED_SET-------------------------------
    inline void setP1(const QPoint &p){x1 = p.x(); y1 = p.y();}      // POINT X1, Y1
    inline void setP1X(int x){x1 = x;}
    inline void setP1Y(int y){y1 = y;}

    inline void setP2(const QPoint &p){x2 = p.x(); y2 = p.y();}      // POINT X2, Y2
    inline void setP2X(int x){x2 = x;}
    inline void setP2Y(int y){y2 = y;}

    inline void setP3(const QPoint &p){x3 = p.x(); y3 = p.y();}      // POINT X3, Y3
    inline void setP3X(int x){x3 = x;}
    inline void setP3Y(int y){y3 = y;}

    inline void setP4(const QPoint &p){x4 = p.x(); y4 = p.y();}      // POINT X4, Y4
    inline void setP4X(int x){x4 = x;}
    inline void setP4Y(int y){y4 = y;}

//--------------------------------------------UTILITY-------------------------------------
    inline int getNumber() const {return number;}
    inline void setNumber(int n) {number = n;}

    inline void setLowBounds(int x = 0, int y = 0) {x_low_bound = x; x_low_bound = y;}
    inline void setHiBounds(int x = 500, int y = 500) {x_hi_bound = x; y_hi_bound = y;}

//--------------------------------------------FUNCTIONAL----------------------------------
    void translate(int dx, int dy);
    bool captured(int x, int y);
    void release();

    void dragNdrop(int dx, int dy);
    void move(int dx, int dy);
    void grubMove(int dx, int dy, uint num);
    void paint(QPainter &painter) const;

protected:
    int x1, x2, x3, x4,
        y1, y2, y3, y4;

    uint number;

    bool capture;
    uint grub_point;

    int x_low_bound, y_low_bound,
        x_hi_bound, y_hi_bound;

    static uint object_count;

    void init();
};

#endif // MYFRAME_H
