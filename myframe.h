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

#include "mytransform.h"

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
    inline uint getP1X() const {return x1;}
    inline uint getP1Y() const {return y1;}

    inline QPoint getP2() const {return QPoint(x2 ,y2);}             // POINT X2, Y2
    inline uint getP2X() const {return x2;}
    inline uint getP12Y() const {return y2;}

    inline QPoint getP3() const {return QPoint(x3 ,y3);}             // POINT X3, Y3
    inline uint getP3X() const {return x3;}
    inline uint getP3Y() const {return y3;}

    inline QPoint getP4() const {return QPoint(x4 ,y4);}             // POINT X4, Y4
    inline uint getP4X() const {return x4;}
    inline uint getP4Y() const {return y4;}

//--------------------------------------------PROTECTED_SET-------------------------------
    inline void setP1(const QPoint &p){x1 = p.x(); y1 = p.y();}      // POINT X1, Y1
    inline void setP1X(uint x){x1 = x;}
    inline void setP1Y(uint y){y1 = y;}

    inline void setP2(const QPoint &p){x2 = p.x(); y2 = p.y();}      // POINT X2, Y2
    inline void setP2X(uint x){x2 = x;}
    inline void setP2Y(uint y){y2 = y;}

    inline void setP3(const QPoint &p){x3 = p.x(); y3 = p.y();}      // POINT X3, Y3
    inline void setP3X(uint x){x3 = x;}
    inline void setP3Y(uint y){y3 = y;}

    inline void setP4(const QPoint &p){x4 = p.x(); y4 = p.y();}      // POINT X4, Y4
    inline void setP4X(uint x){x4 = x;}
    inline void setP4Y(uint y){y4 = y;}

//--------------------------------------------UTILITY-------------------------------------
    inline uint getNumber() const {return number;}
    inline void setNumber(uint n) {number = n;}

//--------------------------------------------FUNCTIONAL----------------------------------
    void translate(int dx, int dy);
    bool captured(uint x, uint y);
    void release();

    void dragNdrop(int dx, int dy);
    void move(int dx, int dy);
    void paint(QPainter &painter) const;

protected:
    uint x1, x2, x3, x4,
         y1, y2, y3, y4;

    uint number;

    bool capture;
    uint grub_point;

    static uint object_count;
};

#endif // MYFRAME_H
