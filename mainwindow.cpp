#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(BTN_SIZE,300);

    in_img = new MyWidget(this);
    in_img->addFrame();

    out_img = new MyWidget(this);
    out_img->addFrame();

    load_btn = new QPushButton(tr("Load img"), this);
    load_btn->setFixedWidth(BTN_SIZE);
    QObject::connect(load_btn,SIGNAL(clicked()),this,SLOT(load_img()));

    transform_btn = new QPushButton(tr("Direct mapping"), this);
    transform_btn->setFixedWidth(BTN_SIZE);
    QObject::connect(transform_btn,SIGNAL(clicked()),this,SLOT(transform_img()));

    transform_back_btn = new QPushButton(tr("Inverse mapping"), this);
    transform_back_btn->setFixedWidth(BTN_SIZE);
    QObject::connect(transform_back_btn,SIGNAL(clicked()),this,SLOT(transform_img_back()));

    prob_btn = new QPushButton(tr("Transform"), this);
    prob_btn->setFixedWidth(BTN_SIZE);
    QObject::connect(prob_btn,SIGNAL(clicked()),this,SLOT(transform_prob()));

    btn_l = new QVBoxLayout();
    main_l = new QHBoxLayout();

    main_l->addWidget(in_img);
    main_l->addWidget(out_img);
    btn_l->addWidget(load_btn);
    btn_l->addWidget(transform_btn);
    btn_l->addWidget(transform_back_btn);
    btn_l->addWidget(prob_btn);
    qobject_cast<QVBoxLayout*>(btn_l)->addStretch();
    createTransformMenu();
    main_l->addItem(btn_l);
    main_l->setGeometry(QRect(0,0,this->width(),this->height()));
}

void MainWindow::createTransformMenu()
{
    l_angle = new QLabel(tr("Angle"), this);
    btn_l->addWidget(l_angle);
    angle = new QSpinBox(this);
    angle->setFixedWidth(BTN_SIZE);
    angle->setMaximum(360);
    angle->setMinimum(-360);
    btn_l->addWidget(angle);

    l_translate_x = new QLabel(tr("Translate X"), this);
    btn_l->addWidget(l_translate_x);
    translate_x  = new QSpinBox(this);
    translate_x->setFixedWidth(BTN_SIZE);
    translate_x->setMaximum(500);
    translate_x->setMinimum(-500);
    btn_l->addWidget(translate_x);

    l_translate_y = new QLabel(tr("Translate Y"), this);
    btn_l->addWidget(l_translate_y);
    translate_y = new QSpinBox(this);
    translate_y->setFixedWidth(BTN_SIZE);
    translate_y->setMaximum(500);
    translate_y->setMinimum(-500);
    btn_l->addWidget(translate_y);
}

MainWindow::~MainWindow()
{
    delete l_angle;
    delete l_translate_x;
    delete l_translate_y;
    delete angle;
    delete translate_x;
    delete translate_y;

    delete load_btn;
    delete transform_btn;
    delete transform_back_btn;
    delete prob_btn;

    delete in_img;
    delete out_img;

    delete main_l;
}

void MainWindow::resizeEvent ( QResizeEvent * event )
{
    main_l->setGeometry(QRect(0,0,this->width(),this->height()));
}

void MainWindow::transform_img_back()
{
    const QImage *image = in_img->getImage();

    if(!image || in_img->getVector()->size() == 0 || out_img->getVector()->size() == 0)
        return;

    QImage *new_img = new QImage(image->width(),image->height(),QImage::Format_RGB32);

    auto v1 = in_img->getVector(), v2 = out_img->getVector();

    QPoint p1(v1->at(0).getP1()), p2(v1->at(0).getP2()),p3(v1->at(0).getP3()),p4(v1->at(0).getP4()),
           op1(v2->at(0).getP1()), op2(v2->at(0).getP2()), op3(v2->at(0).getP3()), op4(v2->at(0).getP4());

    auto ta1 = std::make_tuple(p1, p2, p3),
         ta2 = std::make_tuple(p1, p3, p4),
         tb1 = std::make_tuple(op1, op2, op3),
         tb2 = std::make_tuple(op1, op3, op4);

    MyTransform tr1, tr2;

    tr1.generateFrom3Points(ta1, tb1);
    tr2.generateFrom3Points(ta2, tb2);

    tr1.reverse();
    tr2.reverse();

    uint w = new_img->width(),
         h = new_img->height();

    for(uint y=0;y<h;++y)
        for(uint x=0;x<w;++x)
        {
            QRgb color;
            QPoint pp(x, y);
            bool t1 = MyTransform::pointInTriangle(pp,tb1), t2 = MyTransform::pointInTriangle(pp,tb2);

            if(!(t1 || t2))
            {
                color = qRgb(0,0,0);
                new_img->setPixel(x, y, color);
            }
            if(t1)
            {
                QPoint rp = tr1.transformPoint(pp);
                int rx = rp.x(), ry = rp.y();
                if(rx<0 || ry<0 || rx>w || ry>h)
                    color = qRgb(255,255,255);
                else
                    color = image->pixel(rx, ry);
                new_img->setPixel(x, y, color);
                continue;
            }

            if(t2)
            {
                QPoint rp = tr2.transformPoint(pp);
                int rx = rp.x(), ry = rp.y();
                if(rx<0 || ry<0 || rx>=w || ry>=h)
                    color = qRgb(255,255,255);
                else
                    color = image->pixel(rx, ry);
                new_img->setPixel(x, y, color);
                continue;
            }

        }
    updateImgs(new_img);
}


void MainWindow::transform_img()
{
    const QImage *image = in_img->getImage();

    if(!image || in_img->getVector()->size() == 0 || out_img->getVector()->size() == 0)
        return;

    QImage *new_img = new QImage(image->width(),image->height(),QImage::Format_RGB32);
    QPainter painter;
    painter.begin(new_img);
    QBrush brush(QColor(0,0,0));
        QPen pen(QColor(0,0,0));
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(QRect(0,0,image->width()-1,image->height()-1));
    painter.end();
    auto v1 = in_img->getVector(), v2 = out_img->getVector();

    QPoint p1(v1->at(0).getP1()), p2(v1->at(0).getP2()),p3(v1->at(0).getP3()),p4(v1->at(0).getP4()),
           op1(v2->at(0).getP1()), op2(v2->at(0).getP2()), op3(v2->at(0).getP3()), op4(v2->at(0).getP4());

    auto ta1 = std::make_tuple(p1, p2, p3),
         ta2 = std::make_tuple(p1, p3, p4),
         tb1 = std::make_tuple(op1, op2, op3),
         tb2 = std::make_tuple(op1, op3, op4);

    MyTransform tr1, tr2;

    tr1.generateFrom3Points(ta1, tb1);
    tr2.generateFrom3Points(ta2, tb2);

    uint w = new_img->width(),
         h = new_img->height();

    for(uint y=0;y<h;++y)
        for(uint x=0;x<w;++x)
        {
            QRgb color;
            QPoint pp(x, y);

            bool t1 = MyTransform::pointInTriangle(pp,ta1), t2 = MyTransform::pointInTriangle(pp,ta2);

                        if(t1)
                        {
                            QPoint rp = tr1.transformPoint(pp);
                            int rx = rp.x(), ry = rp.y();
                            if(rx<0 || ry<0 || rx>=w || ry>=h)
                                continue;
                            color = image->pixel(x, y);
                            new_img->setPixel(rx, ry, color);
                            continue;
                        }

                        if(t2)
                        {
                            QPoint rp = tr2.transformPoint(pp);
                            int rx = rp.x(), ry = rp.y();
                            if(rx<0 || ry<0 || rx>=w || ry>=h)
                                continue;
                            color = image->pixel(x, y);
                            new_img->setPixel(rx, ry, color);
                            continue;
                        }

        }
    updateImgs(new_img);
}

void MainWindow::transform_prob()
{
    const QImage *image = in_img->getImage();

    if(!image)
        return;

    QImage *new_img = new QImage(image->width(),image->height(),QImage::Format_RGB32);

    MyTransform tr1;
    int agl = angle->value(),
        tr_x = translate_x->value(),
        tr_y = translate_y->value();
    tr1.rotate(agl);
    tr1.translate(tr_x,tr_y);

    tr1.reverse();

    uint w = new_img->width(),
         h = new_img->height();

    for(uint y=0;y<h;++y)
        for(uint x=0;x<w;++x)
        {
                QPoint pp(x, y);
                QPoint rp = tr1.transformPoint(pp);
                int rx = rp.x(), ry = rp.y();
                QRgb color;
                if(rx<0 || ry<0 || rx>=w || ry>=h)
                    color = qRgb(255,255,255);
                else
                    color = image->pixel(rx, ry);
                new_img->setPixel(x, y, color);
        }
    updateImgs(new_img);
}

void MainWindow::load_img ()
{
    QFileDialog dialog;
    QString name;
    QImage image;

    name = dialog.getOpenFileName(this, tr("Choose img file"),QString(),tr("Images (*.png *.xpm *.jpg *.jpe *.jpeg)"));
    if(!name.isEmpty())
    {
        image = QImage(name);
        QRect desk_r = qApp->desktop()->screenGeometry();
        desk_r.setWidth(desk_r.width()-BTN_SIZE-50);
        desk_r.setHeight(desk_r.height()-50);
        if(desk_r.width()<image.width()*2 || desk_r.height()<image.height())
        {
            QImage temp;
            if(image.width()>image.height())
                temp = image.scaledToWidth(desk_r.width()/2);
            else
                temp = image.scaledToHeight(desk_r.height());

            image = QImage(temp);
        }

        this->setFixedSize(image.width()*2+BTN_SIZE,image.height());

        in_img->setImage(image);
    }
}

void MainWindow::updateImgs(QImage *iii)
{
    out_img->setImage(*iii);
}
