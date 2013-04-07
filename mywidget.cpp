#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    frames.clear();
    frames_save.clear();

    image = 0;
    image_save = 0;
    x_count = 0; y_count = 0;

    m_old_posx = 0;
    m_old_posy = 0;
}

MyWidget::~MyWidget()
{
    if(!image)
        delete image;

    if(!image_save)
        delete image_save;
}

void MyWidget::addFrame()
{
    MyFrame frame(QRect(50,50,50,50));
    frame.setHiBounds(this->width(), this->height());
    frames.push_back(frame);
    frames_save.push_back(frame);
}

void MyWidget::setNet(uint xk, uint yk)
{
    if(xk == 0 || yk == 0)
        return;
    uint fw = width()/xk,
         fh = height()/yk;

    frames.clear();
    frames_save.clear();
    for(uint y=0;y<yk;++y)
        for(uint x=0;x<xk;++x)
        {
            MyFrame fr(QRect(x*fw, y*fh, fw, fh));
            frames_save.push_back(fr);
            frames.push_back(fr);
        }

    x_count = xk;
    y_count = yk;
}

bool MyWidget::event ( QEvent * event )
{
    if(event->type() == QEvent::Resize)
    {
        std::for_each(frames.begin(), frames.end(), [this](MyFrame &frame){frame.setHiBounds(this->width(), this->height());});
        setNet(x_count, y_count);
        return true;
    }
    if(event->type() == QEvent::Paint)
    {
        QPainter painter;

        painter.begin(this);


        if(image)
            painter.drawImage(QRect(0,0,this->width(),this->height()), *image);

        if(frames.size() == 0)
        {
            painter.end();
            return true;
        }

        std::for_each(frames.begin(), frames.end(), [&painter](MyFrame &frame){frame.paint(painter);});

        painter.drawText(QRect(0,0,70,50),QString::number(m_old_posx)+QString(' ')+QString::number(m_old_posy));

        painter.end();

        return true;
    }

    if(frames.size() == 0)
        return true;
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *e = dynamic_cast<QMouseEvent*>(event);
        m_old_posx = e->x(); m_old_posy = e->y();

        if(frames.size() == 0)
            return true;
        for(auto it = frames.rbegin(); it != frames.rend(); it++)
        {
            if((*it).captured(m_old_posx ,m_old_posy))
            {
                setCursor(Qt::ClosedHandCursor);
                //break;
            }
        }
        this->update();
        return true;
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(frames.size() == 0)
            return true;

        setCursor(Qt::ArrowCursor);
        std::for_each(frames.rbegin(), frames.rend(), [](MyFrame &frame){frame.release();});
        transform_img_back_bilinear();

        return true;
    }
    if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent *e = dynamic_cast<QMouseEvent*>(event);
        int x = e->x(), y = e->y();

        if(x<0)
            x = 0;
        if(y<0)
            y = 0;
        if(x>=this->width())
            x = this->width()-1;
        if(y>=this->height())
            y = this->height()-1;

        int dx = (int)x-m_old_posx, dy = (int)y-m_old_posy;

        std::for_each(frames.rbegin(), frames.rend(), [dx, dy](MyFrame &frame){frame.dragNdrop(dx, dy);});

        m_old_posx = x; m_old_posy = y;
        this->update();

        return true;
    }

    return true;
}

void MyWidget::transform_img()
{
    if(image)
        delete image;

    image = transform();
    this->update();
}

void MyWidget::transform_img_back()
{
    if(image)
        delete image;

    image = transform_back();
    this->update();
}

QImage* MyWidget::transform() const
{
    if(!image_save || frames.size() == 0 || frames_save.size() == 0)
        return image_save;

    uint w = image_save->width(),
         h = image_save->height();

    QImage *new_img = new QImage(w,h,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(new_img);
    QBrush brush(QColor(0,0,0));
        QPen pen(QColor(0,0,0));
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(QRect(0,0,w-1,h-1));
    painter.end();


    for(uint i=0;i<frames.size();++i)
    {
        auto v2 = &frames, v1 = &frames_save;
        QPoint p1(v1->at(i).getPoint(0)), p2(v1->at(i).getPoint(1)),p3(v1->at(i).getPoint(2)),p4(v1->at(i).getPoint(3)),
               op1(v2->at(i).getPoint(0)), op2(v2->at(i).getPoint(1)), op3(v2->at(i).getPoint(2)), op4(v2->at(i).getPoint(3));

        std::vector<QPoint>
             ta1{p1, p2, p3},
             ta2{p1, p3, p4},
             tb1{op1, op2, op3},
             tb2{op1, op3, op4};

        AffineTransform tr1, tr2;

        tr1.generateFromPoints(ta1, tb1);
        tr2.generateFromPoints(ta2, tb2);

        for(uint y=0;y<h;++y)
            for(uint x=0;x<w;++x)
            {
                QRgb color;
                QPoint pp(x, y);

                bool t1 = ATransform::pointInTriangle(pp,ta1), t2 = ATransform::pointInTriangle(pp,ta2);

                            if(t1)
                            {
                                QPointF rp = tr1.transformPoint(pp);
                                int rx = rp.x(), ry = rp.y();
                                if(rx<0 || ry<0 || rx>=w || ry>=h)
                                    continue;
                                color = image_save->pixel(x, y);
                                new_img->setPixel(rx, ry, color);
                                continue;
                            }

                            if(t2)
                            {
                                QPointF rp = tr2.transformPoint(pp);
                                int rx = rp.x(), ry = rp.y();
                                if(rx<0 || ry<0 || rx>=w || ry>=h)
                                    continue;
                                color = image_save->pixel(x, y);
                                new_img->setPixel(rx, ry, color);
                                continue;
                            }
            }
    }
    return new_img;
}
QImage* MyWidget::transform_back() const
{
    if(!image_save || frames.size() == 0 || frames_save.size() == 0)
        return image_save;

    uint w = image_save->width(),
         h = image_save->height();

    QImage *new_img = new QImage(w,h,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(new_img);
    QBrush brush(QColor(0,0,0));
        QPen pen(QColor(0,0,0));
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(QRect(0,0,w-1,h-1));
    painter.end();


    for(uint i=0;i<frames.size();++i)
    {
        auto v2 = &frames, v1 = &frames_save;
        QPoint p1(v1->at(i).getPoint(0)), p2(v1->at(i).getPoint(1)),p3(v1->at(i).getPoint(2)),p4(v1->at(i).getPoint(3)),
               op1(v2->at(i).getPoint(0)), op2(v2->at(i).getPoint(1)), op3(v2->at(i).getPoint(2)), op4(v2->at(i).getPoint(3));

        std::vector<QPoint>
             //ta1{p1, p2, p3},
             //ta2{p1, p3, p4},
             //tb1{op1, op2, op3},
             //tb2{op1, op3, op4},
             vpi{p1,p2,p3,p4},
             vpo{op1,op2,op3,op4};

        //AffineTransform tr1, tr2;
        PerspectiveTransform pt;

        pt.generateFromPoints(vpo, vpi);

        //tr1.generateFromPoints(tb1, ta1);
        //tr2.generateFromPoints(tb2, ta2);

        for(uint y=0;y<h;++y)
            for(uint x=0;x<w;++x)
            {
                QRgb color;
                QPoint pp(x, y);
                /*bool t1 = ATransform::pointInTriangle(pp,tb1), t2 = ATransform::pointInTriangle(pp,tb2);

                if(!(t1 || t2))
                {
                    continue;
                }
                if(t1)
                {*/
                if(v2->at(i).containsPoint(pp))
                {
                    //QPointF rp = tr1.transformPoint(pp);
                    QPointF rp = pt.transformPoint(pp);
                    double rx = rp.x(), ry = rp.y();
                    if(rx<0 || ry<0 || rx>=w || ry>=h)
                        color = qRgb(255,255,255);
                    else
                        color = image_save->pixel(rx, ry);
                    new_img->setPixel(x, y, color);
                }
                    //continue;
                /*}

                if(t2)
                {
                    QPointF rp = tr2.transformPoint(pp);
                    int rx = rp.x(), ry = rp.y();
                    if(rx<0 || ry<0 || rx>=w || ry>=h)
                        color = qRgb(255,255,255);
                    else
                        color = image_save->pixel(rx, ry);
                    new_img->setPixel(x, y, color);
                    continue;
                }
*/
            }
    }
    return new_img;
}

void MyWidget::transform_img_back_bilinear()
{
    if(image)
        delete image;

    image = transform_back_bilinear();
    this->update();
}

QImage* MyWidget::transform_back_bilinear() const
{
    if(!image_save || frames.size() == 0 || frames_save.size() == 0)
        return image_save;

    uint w = image_save->width(),
         h = image_save->height();

    QImage *new_img = new QImage(w,h,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(new_img);
    QBrush brush(QColor(0,0,0));
        QPen pen(QColor(0,0,0));
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(QRect(0,0,w-1,h-1));
    painter.end();


    for(uint i=0;i<frames.size();++i)
    {
        auto v2 = &frames, v1 = &frames_save;
        QPoint p1(v1->at(i).getPoint(0)), p2(v1->at(i).getPoint(1)),p3(v1->at(i).getPoint(2)),p4(v1->at(i).getPoint(3)),
               op1(v2->at(i).getPoint(0)), op2(v2->at(i).getPoint(1)), op3(v2->at(i).getPoint(2)), op4(v2->at(i).getPoint(3));

        std::vector<QPoint>
             vpi{p1,p2,p3,p4},
             vpo{op1,op2,op3,op4};

        BilineTransform pt;

        pt.generateFromPoints(vpo, vpi);

        for(uint y=0;y<h;++y)
            for(uint x=0;x<w;++x)
            {
                QRgb color;
                QPoint pp(x, y);
                if(v2->at(i).containsPoint(pp))
                {
                    QPointF rp = pt.transformPoint(pp);
                    double rx = rp.x(), ry = rp.y();
                    if(rx<0 || ry<0 || rx>=w || ry>=h)
                        color = qRgb(255,255,255);
                    else
                        color = image_save->pixel(rx, ry);

                    new_img->setPixel(x, y, color);
                }
            }
    }
    return new_img;
}
