#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    out_image = 0;
    image = 0;

    this->setGeometry(QRect(100,100,400,300));

    in_img = new MyWidget(this);
    in_img->installEventFilter(this);

    out_img = new MyWidget(this);
    out_img->installEventFilter(this);

    load_btn = new QPushButton(tr("Load img"), this);
    load_btn->setFixedWidth(80);
    QObject::connect(load_btn,SIGNAL(clicked()),this,SLOT(load_img()));

    transform_btn = new QPushButton(tr("Transform"), this);
    transform_btn->setFixedWidth(80);
    QObject::connect(transform_btn,SIGNAL(clicked()),this,SLOT(transform_img()));

    prob_btn = new QPushButton(tr("Check"), this);
    prob_btn->setFixedWidth(80);
    QObject::connect(prob_btn,SIGNAL(clicked()),this,SLOT(transform_prob()));

    btn_l = new QVBoxLayout();
    main_l = new QHBoxLayout();

    main_l->addWidget(in_img);
    main_l->addWidget(out_img);
    btn_l->addWidget(load_btn);
    btn_l->addWidget(transform_btn);
    btn_l->addWidget(prob_btn);
    main_l->addItem(btn_l);
    main_l->setGeometry(QRect(0,0,this->width(),this->height()));
}

MainWindow::~MainWindow()
{
    if(image)
    {
        image->save(tr("input.jpg"));
        delete image;
    }

    if(out_image)
    {
        out_image->save(tr("dis.jpg"));
        delete out_image;
    }

    delete load_btn;
    delete transform_btn;
    delete prob_btn;

    delete in_img;
    delete out_img;

    delete main_l;
}

void MainWindow::resizeEvent ( QResizeEvent * event )
{
    main_l->setGeometry(QRect(0,0,this->width(),this->height()));
}

bool MainWindow::eventFilter( QObject * obj, QEvent * event )
{
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *e = dynamic_cast<QMouseEvent*>(event);
        int x = e->x(), y = e->y();

        MyWidget * w = qobject_cast<MyWidget*>(obj);

        if(!w)
            return false;

        auto vector = w->getVector();
        uint cl = w->getClickNum();

        if(cl == 4)
        {
            vector->clear();
            w->zeroClickNum();
        }

        vector->push_back(QPoint(x, y));
        w->incClickNum();

        w->update();
    }
    if(event->type() == QEvent::Paint)
    {
        if( obj == in_img )
            drawWidget(in_img, image);

        if( obj == out_img )
            drawWidget(out_img, out_image);

    }
    return true;
}

void MainWindow::drawWidget(QWidget *wdg, QImage *img)
{
    QPainter painter;

    painter.begin(wdg);

    if(img)
        painter.drawImage(QRect(0,0,wdg->width(),wdg->height()), *img);

    MyWidget * w = qobject_cast<MyWidget*>(wdg);
    if(!w)
    {
        painter.end();
        return;
    }

    auto v = w->getVector();

    if(v->size() == 0)
    {
        painter.end();
        return;
    }

    painter.setPen(QColor(0,0,255));

    QPoint *pp = &(*(v->begin()));
    //int x = pp->x(), y = pp->y();
    //painter.drawRect(pp->x()-2,pp->y()-2, 4, 4);
    //painter.drawText(pp->x()-4,pp->y()-4, QString("1"));

    uint k = 1;//, x = pp->x(), y = pp->y();

    for(auto i=v->begin(); i!=v->end(); ++i)
    {
        QPoint *np = &(*(i));
        painter.drawRect(np->x()-2,np->y()-2, 4, 4);
        painter.drawLine(pp->x(),pp->y(),np->x(),np->y());
        painter.drawText(np->x()-4,np->y()-4, QString::number(k));

        //x = np->x(), y = np->y();

        pp = np;

        ++k;
    }

    if(v->size() == 4)
    {
        QPoint *fp = &(*(v->begin()));
        painter.drawLine(pp->x(),pp->y(),fp->x(),fp->y());
    }

    painter.end();
}

void MainWindow::transform_img()
{
    if(!image || in_img->getClickNum()<4 || out_img->getClickNum()<4)
        return;

    QImage *new_img = new QImage(image->width(),image->height(),QImage::Format_RGB32);

    double mw = in_img->width()/image->width(),
           mh = in_img->height()/image->height();

    auto v1 = in_img->getVector(), v2 = out_img->getVector();

    auto ta1 = std::make_tuple(QPoint(v1->at(0).x(),v1->at(0).y()), QPoint(v1->at(1).x(),v1->at(1).y()), QPoint(v1->at(2).x(),v1->at(2).y())),
         ta2 = std::make_tuple(QPoint(v1->at(0).x(),v1->at(0).y()), QPoint(v1->at(2).x(),v1->at(2).y()), QPoint(v1->at(3).x(),v1->at(3).y())),
         tb1 = std::make_tuple(QPoint(v2->at(0).x(),v2->at(0).y()), QPoint(v2->at(1).x(),v2->at(1).y()), QPoint(v2->at(2).x(),v2->at(2).y())),
         tb2 = std::make_tuple(QPoint(v2->at(0).x(),v2->at(0).y()), QPoint(v2->at(2).x(),v2->at(2).y()), QPoint(v2->at(3).x(),v2->at(3).y()));
    //std::set<0>(ta1, v1->at(0)); std::set<1>(ta1, v1->at(1)); std::set<2>(ta1, v1->at(2));

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
            if(MyTransform::pointInTriangle(pp,tb1))
            {
                QPoint rp = tr1.transformPoint(pp);
                int rx = rp.x(), ry = rp.y();
                if(rx<0 || ry<0 || rx>=w || ry>=h)
                    color = qRgb(255,255,255);
                else
                    color = image->pixel(rx, ry);

            }
            else
                color = qRgb(255,255,255);
            new_img->setPixel(x, y, color);
        }
    updateImgs(new_img);
}

void MainWindow::transform_prob()
{
    if(!image)
        return;

    QImage *new_img = new QImage(image->width(),image->height(),QImage::Format_RGB32);

    MyTransform tr1;
    tr1.rotate(30);
    tr1.translate(300,50);
    //tr1.scale(0.5);

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

    name = dialog.getOpenFileName(this, tr("Choose img file"),QString(),tr("Images (*.png *.xpm *.jpg)"));
    if(!name.isEmpty())
    {
        image = new QImage(name);
        in_img->update();
    }
}

void MainWindow::updateImgs(QImage *iii)
{
    if( out_image )
        delete out_image;

    out_image = iii;

    out_img->update();
}
