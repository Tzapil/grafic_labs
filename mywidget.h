#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QSize>

#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

#include "myframe.h"

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);
    
    std::vector<MyFrame>* getVector();
signals:
    
public slots:
    
private:
    std::vector<MyFrame> frames;

protected:
    void resizeEvent ( QResizeEvent * event );
    bool event ( QEvent * event );
};

#endif // MYWIDGET_H
