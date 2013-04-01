#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QImage>
#include <QSize>

#include <QEvent>
#include <QResizeEvent>
#include <QMouseEvent>

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

    void addFrame();
    inline void setImage(const QImage &i) {if(image) delete image;image = new QImage(i);update();}
    inline const QImage* getImage(){return image;}

    inline const std::vector<MyFrame>* getVector(){return &frames;}

    ~MyWidget();
signals:
    
public slots:
    
private:
    std::vector<MyFrame> frames;

    QImage *image;

    uint m_old_posx, m_old_posy;
protected:
    void resizeEvent ( QResizeEvent * event );
    bool event ( QEvent * event );
};

#endif // MYWIDGET_H
