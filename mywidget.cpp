#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    frames.clear();
}

std::vector<MyFrame>* MyWidget::getVector()
{
    return &frames;
}

void MyWidget::resizeEvent ( QResizeEvent * event )
{
    QSize size(event->oldSize());
    int dx = this->width()  - size.width(),
        dy = this->height() - size.height();
    std::for_each(frames.begin(), frames.end(), [](MyFrame &frame){frame.translate(dx ,dy);});
}

bool MyWidget::event ( QEvent * event )
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *e = dynamic_cast<QMouseEvent*>(event);
        int x = e->x(), y = e->y();

        if(frames.size() == 0)
            return true;
        for(auto it = frames.rbegin(); it != frames.rend(); it++)
        {
            if((*it).captured(x ,y))
                break;
        }
        this->update();
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(frames.size() == 0)
            return true;
        std::for_each(frames.rbegin(), frames.rend(), [](MyFrame &frame){frame.release();});
        this->update();
    }

    return true;
}
