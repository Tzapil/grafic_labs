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
#include "atransform.h"
#include "perspectivetransform.h"
#include "bilinetransform.h"

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);

    void addFrame();

    void setNet(uint xk, uint yk);
    inline void setImage(const QImage &i) {
        if(image)
            delete image;
        image = new QImage(i);
        image_save = new QImage(i);
        update();
    }
    inline const QImage* getImage(){return image;}

    inline const std::vector<MyFrame>* getVector(){return &frames;}
    inline void restoreImage() {if(image) delete image; image = new QImage(*image_save);}

    void transform_img();
    void transform_img_back();
    void transform_img_back_bilinear();

    QImage* transform() const;
    QImage* transform_back() const;
    QImage* transform_back_bilinear() const;

    ~MyWidget();
signals:
    
public slots:
    
private:
    std::vector<MyFrame> frames_save;
    std::vector<MyFrame> frames;

    QImage *image_save,
           *image;

    uint m_old_posx, m_old_posy;

    uint x_count, y_count;
protected:
    bool event ( QEvent * event );
};

#endif // MYWIDGET_H
