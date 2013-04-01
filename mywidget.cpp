#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    frames.clear();

    image = 0;
}

MyWidget::~MyWidget()
{
    if(!image)
        delete image;
}

void MyWidget::resizeEvent ( QResizeEvent * event )
{
    const QSize &old_size = event->oldSize(),
                &new_size = event->size();
    int dx = new_size.width()  - old_size.width(),
        dy = new_size.height() - old_size.height();
    std::for_each(frames.begin(), frames.end(), [dx, dy](MyFrame &frame){frame.translate(dx ,dy);});
}

void MyWidget::addFrame()
{
    frames.push_back(MyFrame(QRect(50,50,50,50)));
}

bool MyWidget::event ( QEvent * event )
{
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
                break;
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
        this->update();

        return true;
    }
    if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent *e = dynamic_cast<QMouseEvent*>(event);
        uint x = e->x(), y = e->y();

        int dx = (int)x-m_old_posx, dy = (int)y-m_old_posy;

        std::for_each(frames.rbegin(), frames.rend(), [dx, dy](MyFrame &frame){frame.dragNdrop(dx, dy);});

        m_old_posx = x; m_old_posy = y;
        this->update();

        return true;
    }

    return true;
}
