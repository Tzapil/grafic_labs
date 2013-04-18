#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(BTN_SIZE,300);

    in_img = new MyWidget(this);
    in_img->setNet(6, 6);

    out_img = new MyWidget(this);
    out_img->setNet(3, 3);

    load_btn = new QPushButton(tr("Load img"), this);
    load_btn->setFixedWidth(BTN_SIZE);
    QObject::connect(load_btn,SIGNAL(clicked()),this,SLOT(load_img()));

    load_second_btn = new QPushButton(tr("Load second img"), this);
    load_second_btn->setFixedWidth(BTN_SIZE);
    QObject::connect(load_second_btn,SIGNAL(clicked()),this,SLOT(load_second_img()));

    morf_btn = new QPushButton(tr("Morfing"), this);
    morf_btn->setFixedWidth(BTN_SIZE);
    QObject::connect(morf_btn,SIGNAL(clicked()),this,SLOT(morfing()));

    btn_l = new QVBoxLayout();
    main_l = new QHBoxLayout();

    main_l->addWidget(in_img);
    main_l->addWidget(out_img);
    btn_l->addWidget(load_btn);
    btn_l->addWidget(load_second_btn);
    btn_l->addWidget(morf_btn);
    qobject_cast<QVBoxLayout*>(btn_l)->addStretch();
    createTransformMenu();
    main_l->addItem(btn_l);
    main_l->setGeometry(QRect(0,0,this->width(),this->height()));
}

void MainWindow::createTransformMenu()
{
    l_cadres = new QLabel(tr("Steps"), this);
    btn_l->addWidget(l_cadres);
    cadres = new QSpinBox(this);
    cadres->setFixedWidth(BTN_SIZE);
    cadres->setMaximum(360);
    cadres->setMinimum(2);
    btn_l->addWidget(cadres);
}

MainWindow::~MainWindow()
{
    delete l_cadres;

    delete load_btn;
    delete load_second_btn;
    delete morf_btn;

    delete in_img;
    delete out_img;

    delete main_l;
}

void MainWindow::resizeEvent ( QResizeEvent * event )
{
    main_l->setGeometry(QRect(0,0,this->width(),this->height()));
}

void MainWindow::load_img ()
{
    load_img_on_wdg(*in_img);
}

void MainWindow::load_second_img()
{
    load_img_on_wdg(*out_img);
}

void MainWindow::load_img_on_wdg(QWidget &w)
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
        (qobject_cast<MyWidget*>(&w))->setImage(image);
        //in_img->setImage(image);
    }
}
void MainWindow::morfing()
{
    std::vector<QImage> iv;
    int steps = cadres->value();
    auto old_start_in_v = (in_img->getSaveVector()),
         finish_in_v = (in_img->getVector()),
         old_start_out_v = (out_img->getSaveVector()),
         finish_out_v = (out_img->getVector());
    std::vector<MyFrame> _in_v,
                         _out_v;

    QImage *start_in_i = new QImage(*in_img->getSaveImage()),
           *start_out_i = new QImage(*out_img->getSaveImage());

    uint w = start_in_i->width(),
         h = start_in_i->height();

    for(int i=0;i<=(steps-1);++i)
    {
        double k = (double)i/(steps-1);
        _in_v.clear();_out_v.clear();

        for(int k=0;k<old_start_in_v->size();++k)
        {
            MyFrame fr(old_start_in_v->at(k)),
                    res(finish_in_v->at(k));
            for(int l=0;l<fr.pointNumber();++l)
            {
                QPoint p = fr.getPoint(l),
                       z = res.getPoint(l);
                double stx = (double)(p.x()-z.x())/(steps-1),
                       sty = (double)(p.y()-z.y())/(steps-1);
                fr.setPoint(QPoint(p.x()-stx*i,p.y()-sty*i), l);
            }
            _in_v.push_back(fr);

            MyFrame ofr(old_start_out_v->at(k)),
                    ores(finish_out_v->at(k));
            for(int l=0;l<ofr.pointNumber();++l)
            {
                QPoint p = ofr.getPoint(l),
                       z = ores.getPoint(l);
                double stx = (double)(p.x()-z.x())/(steps-1),
                       sty = (double)(p.y()-z.y())/(steps-1);
                QPoint pp = QPoint(z.x()+stx*i,z.y()+sty*i);
                ofr.setPoint(pp, l);
            }
            _out_v.push_back(ofr);
        }

        QImage *in_image = bilineTransformImg(start_in_i, old_start_in_v, &_in_v);
        QImage *out_image = bilineTransformImg(start_out_i, old_start_out_v, &_out_v);
        QImage *new_img = new QImage(w,h,QImage::Format_RGB32);

        for(int x=0;x<w;++x)
            for(int y=0;y<h;++y)
            {
                QColor ip = QColor(in_image->pixel(x, y)),
                       op = QColor(out_image->pixel(x, y));
                double red = (1.0-k)*ip.red()+k*op.red(),
                       green = (1.0-k)*ip.green()+k*op.green(),
                       blue = (1.0-k)*ip.blue()+k*op.blue();
                new_img->setPixel(x, y, qRgb(red, green, blue));
            }
        iv.push_back(*new_img);

        in_image->save(QString("biline_in")+QString::number(i)+QString(".jpg"));
        out_image->save(QString("biline_out")+QString::number(i)+QString(".jpg"));
        new_img->save(QString("biline_result")+QString::number(i)+QString(".jpg"));
        delete in_image;
        delete out_image;
        delete new_img;
    }

    delete start_in_i;
    delete start_out_i;
}

QImage* MainWindow::bilineTransformImg(const QImage *image_save,const std::vector<MyFrame>* frames,const std::vector<MyFrame>* frames_save)
{

    uint w = image_save->width(),
         h = image_save->height();

    QImage *new_img = new QImage(w,h,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(new_img);
    QBrush brush(QColor(0,0,0));
        QPen pen(QColor(0,0,0));
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawRect(QRect(0,0,w-1,h-1));
    painter.end();


    for(uint i=0;i<frames->size();++i)
    {
        auto v1 = frames, v2 = frames_save;
        QPoint p1(v1->at(i).getPoint(0)), p2(v1->at(i).getPoint(1)),p3(v1->at(i).getPoint(2)),p4(v1->at(i).getPoint(3)),
               op1(v2->at(i).getPoint(0)), op2(v2->at(i).getPoint(1)), op3(v2->at(i).getPoint(2)), op4(v2->at(i).getPoint(3));

        std::vector<QPoint>
             vpi{p1,p2,p3,p4},
             vpo{op1,op2,op3,op4};

        BilineTransform pt;

        pt.generateFromPoints(vpo, vpi);

        uint minx = w, maxx = 0,
             miny = h, maxy = 0;

        std::for_each(vpo.begin(), vpo.end(),
        [&minx, &maxx, &miny, &maxy](QPoint p){
            if(maxx<p.x())
                maxx = p.x();
            if(minx>p.x())
                minx = p.x();
            if(maxy<p.y())
                maxy = p.y();
            if(miny>p.y())
                miny = p.y();
        });

        for(uint y=miny;y<=maxy;++y)
            for(uint x=minx;x<=maxx;++x)
            {
                QRgb color;
                QPoint pp(x, y);
                if(v2->at(i).containsPoint(pp))
                {
                    QPointF rp = pt.transformPoint(pp);
                    double rx = rp.x(), ry = rp.y();
                    if(rx<0 || ry<0 || rx>=w || ry>=h)
                        color = qRgb(255,255,255);
                    else
                        color = image_save->pixel(rx, ry);

                    new_img->setPixel(x, y, color);
                }
            }
    }
    return new_img;
}

void MainWindow::updateImgs(QImage *iii)
{
    out_img->setImage(*iii);
}
