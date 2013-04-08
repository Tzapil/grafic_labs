#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QDesktopWidget>

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
#include <functional>
#include <algorithm>
#include <iterator>

#include "mywidget.h"
#include "atransform.h"
#include "affinetransform.h"
#include "bilinetransform.h"
#include "myframe.h"

#include "qapplication.h"

#define BTN_SIZE 150

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent ( QResizeEvent * event );

    void updateImgs(QImage *iii);
private slots:
    void load_img ();
    void load_second_img();
    void load_img_on_wdg(QWidget &w);
    void morfing();
private:
    QPushButton *load_btn,
                *load_second_btn,
                *morf_btn;
    MyWidget *in_img,
            *out_img;

    QLayout *main_l,*btn_l;

    QLabel *l_cadres;
    QSpinBox *cadres;

    void createTransformMenu();

    QImage* bilineTransformImg(QImage *image_save, std::vector<MyFrame>* frames, std::vector<MyFrame>* frames_save);
};

#endif // MAINWINDOW_H
