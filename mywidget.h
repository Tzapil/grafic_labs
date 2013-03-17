#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

#include <vector>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);
    
    std::vector<QPoint>* getVector();

    uint getClickNum();
    void zeroClickNum();
    void setClickNum(uint k);
    void incClickNum();
signals:
    
public slots:
    
private:
    std::vector<QPoint> points;
    uint click_num;
};

#endif // MYWIDGET_H
