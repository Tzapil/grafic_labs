#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>

#include <QFileDialog>

#include <QLayout>
#include <QFormLayout>

#include <QPainter>
#include <QImage>
#include <QBrush>
#include <QColor>
#include <QRgb>
#include <QPen>
#include <QRect>
#include <QEvent>
#include <QMouseEvent>
#include <QTransform>

#include <vector>

#include "mywidget.h"
#include "mytransform.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent ( QResizeEvent * event );
    bool eventFilter( QObject * obj, QEvent * event );

    void drawWidget(QWidget *wdg, QImage *img=0);

    void updateImgs(QImage *iii);
private slots:
    void load_img ();
    void transform_img();
    void transform_prob();
private:
    QImage *image,
           *out_image;
    QPushButton *load_btn,
                *transform_btn,
                *prob_btn;
    MyWidget *in_img,
            *out_img;

    QLayout *main_l,*btn_l;
};

#endif // MAINWINDOW_H
