#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <configdialog.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    //setCentralWidget(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    setFixedSize(800,600);
    //pipe = new Pipe();
    //scene->addItem(pipe);
    //QBrush redBrush(Qt::red);
    //QBrush blueBrush(Qt::blue);

    //QPen blackpen(Qt::black);

    //blackpen.setWidth(6);

    //ellipse = scene->addEllipse(10,10,100,100,blackpen,redBrush);
    //rectangle = scene->addRect(0,0,50,50,blackpen,blueBrush);
    //->setFlag(QGraphicsItem::ItemIsSelectable);
    //rectangle->setFlag(QGraphicsItem::ItemIsPanel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConfig_triggered()
{
    ConfigDialog configDialog(this);
    connect(&configDialog,SIGNAL(finish(int,int)),this,SLOT(resize(int,int)));
    configDialog.work();
}
void MainWindow::resize(int a,int b)
{
    scene->clear();
    height=a;
    weight=b;
    qDebug()<<height<<" "<<weight;

    setFixedSize(height*150,weight*150);
    double left = -100*weight/2;
    double top = -100*height/2;
    for(int i=0;i<height-1;i++)
    {
        for(int j=0;j<weight;j++)
        {
            pipe[0][i][j]=new Pipe(left+100*i,top+100*j,120,20,1);
            scene->addItem(pipe[0][i][j]);
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<weight-1;j++)
        {
            pipe[1][i][j]=new Pipe(left+100*i,top+100*j,20,120,1);
            scene->addItem(pipe[1][i][j]);
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<weight;j++)
        {
            pipe[2][i][j]=new Pipe(left+100*i,top+100*j,20,20,0);
            scene->addItem(pipe[2][i][j]);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *e){
   /* //QPainter painter(this);
    QPen paintpen (Qt::red);
    paintpen.setWidth(6);

    QPen linepen(Qt::blue);
    linepen.setWidth(2);

    QPoint p1;
    p1.setX(20);
    p1.setX(20);

    QPoint p2;
    p2.setX(100);
    p2.setY(100);

    //painter.setPen(linepen);
   // painter.drawLine(p1,p2);

    //painter.setPen(paintpen);
    //painter.drawPoint(p1);
    //painter.drawPoint(p2);
    scene->addLine(QLineF(p1,p2),linepen);*/
}
