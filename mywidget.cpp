#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    click_num = 0;
    points.clear();
}

std::vector<QPoint>* MyWidget::getVector()
{
    return &points;
}

uint MyWidget::getClickNum()
{
    return click_num;
}

void MyWidget::setClickNum(uint k)
{
    click_num = k;
}

void MyWidget::incClickNum()
{
    click_num++;
}

void MyWidget::zeroClickNum()
{
    click_num = 0;
}
