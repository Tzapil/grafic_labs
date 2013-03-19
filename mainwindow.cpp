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

    main_l = new QHBoxLayout(this);
    main_l->addWidget(in_img);
    main_l->addWidget(out_img);
    main_l->addWidget(load_btn);
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
