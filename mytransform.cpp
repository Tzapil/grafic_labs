#include "mytransform.h"

MyTransform::MyTransform()
{
    drop();
}

void MyTransform::translate(double dx, double dy)
{
    l += dx;
    m += dy;
}

void MyTransform::scale(double sx, double sy)
{
    a *= sx; c *= sx; l *= sx;
    b *= sy; d *= sy; m *= sy;
}

void MyTransform::rotate(double angle)
{
    double rad = angle*M_PI/180,
           sn = sin(rad),
           cs = cos(rad);

    a = cs*a - sn*b; b = sn*a + cs*b;
    c = cs*c - sn*d; d = sn*c + cs*d;
    l = cs*l - sn*m; m = sn*l + cs*m;
}

void MyTransform::shear(double shx, double shy)
{
    double mult = (shx*shy+1);

    a += b*shx; b = a*shy + b*mult;
    c += d*shx; d = c*shy + d*mult;
    c += m*shx; m = l*shy + m*mult;
}

QPoint MyTransform::transformPoint(const QPoint &point)
{

    int x = point.x(), y = point.y(),
        new_x = a*x+c*y+l,
        new_y = b*x+d*y+m;

    return QPoint(new_x, new_y);
}

void MyTransform::generateFrom3Points(std::tuple<QPoint, QPoint, QPoint> in_points, std::tuple<QPoint, QPoint, QPoint> out_points)
{
    QPoint *ip1 = &(std::get<0>(in_points)),
           *ip2 = &(std::get<1>(in_points)),
           *ip3 = &(std::get<2>(in_points)),

           *op1 = &(std::get<0>(out_points)),
           *op2 = &(std::get<1>(out_points)),
           *op3 = &(std::get<2>(out_points));

   long x_0 = ip1->x(), x_1 = ip2->x(), x_2 = ip3->x(),
        y_0 = ip1->y(), y_1 = ip2->y(), y_2 = ip3->y(),

        ox_0 = op1->x(), ox_1 = op2->x(), ox_2 = op3->x(),
        oy_0 = op1->y(), oy_1 = op2->y(), oy_2 = op3->y(),

        dy12 = y_1-y_2, dy20 = y_2-y_0, dy01 = y_0-y_1,
        dx21 = x_2-x_1, dx02 = x_0-x_2, dx10 = x_1-x_0,

        dxy12 = x_1*y_2-x_2*y_1, dxy20 = x_2*y_0-x_0*y_2, dxy01 = x_0*y_1-x_1*y_0;


    double det = 1.0/(x_0*dy12-y_0*(x_1-x_2)+dxy12);

    a = (dy12*ox_0 + dy20*ox_1 + dy01*ox_2)*det;
    b = (dy12*oy_0 + dy20*oy_1 + dy01*oy_2)*det;
    c = (dx21*ox_0 + dx02*ox_1 + dx10*ox_2)*det;
    d = (dx21*oy_0 + dx02*oy_1 + dx10*oy_2)*det;

    l = (dxy12*ox_0 + dxy20*ox_1 + dxy01*ox_2)*det;
    m = (dxy12*oy_0 + dxy20*oy_1 + dxy01*oy_2)*det;
}


void MyTransform::drop()
{
   a = 1; b = 0; c = 0; d = 1; l = 0; m = 0;
}

void MyTransform::reverse()
{
    double _a = a, _b = b, _c = c, _d = d, _l = l, _m = m,
           det = 1/(a*d-b*c);

    a = _d*det; b = -_b*det;
    c = -_c*det; d = _a*det;
    l = det*(_c*_m-_l*_d); m = det*(_l*_b-_a*_m);
}

QImage* MyTransform::transformImage(QImage &in_img)
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
