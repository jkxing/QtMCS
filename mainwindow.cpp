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
    connect(&configDialog,SIGNAL(finish(int,int,int,int,int,int)),this,SLOT(resize(int,int,int,int,int,int)));
    configDialog.work();
}
void MainWindow::resize(int len,int in1,int in2,int out1,int out2,int out3)
{
    setFixedSize(len*150+150,len*150+150);
    scene->clear();
    length=len;
    input[0]=in1;
    input[1]=in2;
    output[0]=out1;
    output[1]=out2;
    output[2]=out3;
    double left = -100*length/2;
    double top = -100*length/2;
    for(int i=0;i<length-1;i++)
    {
        for(int j=0;j<length;j++)
        {
            pipe[0][i][j]=new Pipe(left+100*i,top+100*j,120,20,-1);
            scene->addItem(pipe[0][i][j]);
        }
    }
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length-1;j++)
        {
            pipe[1][i][j]=new Pipe(left+100*i,top+100*j,20,120,-1);
            scene->addItem(pipe[1][i][j]);
        }
    }
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length;j++)
        {
            pipe[2][i][j]=new Pipe(left+100*i,top+100*j,20,20,1);
            scene->addItem(pipe[2][i][j]);
        }
    }
    inpipe[0]=new Pipe(left+in1*100,top-100,20,120,1);
    inpipe[1]=new Pipe(left+in2*100,top-100,20,120,1);
    outpipe[0]=new Pipe(left+out1*100,top+100*length-100,20,120,1);
    outpipe[1]=new Pipe(left+out2*100,top+100*length-100,20,120,1);
    outpipe[2]=new Pipe(left+out3*100,top+100*length-100,20,120,1);
    for(int i=0;i<2;i++)
        scene->addItem(inpipe[i]);
    for(int i=0;i<3;i++)
        scene->addItem(outpipe[i]);
}

void MainWindow::calculate()
{
    int input1 = ui->lineEdit->text().toInt();
    int input2 = ui->lineEdit_2->text().toInt();
    vector<double> len;
    len.clear();
    for(int i=0;i<length;i++)
        for(int j=0;j<length-1;j++)
        {
            if(pipe[1][i][j]->getState())
                len.push_back(1);
            else
                len.push_back(0);
        }
    for(int i=0;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(pipe[0][i][j]->getState())
                len.push_back(1);
            else
                len.push_back(0);
        }
    for(int i=0;i<5;i++)
        len.push_back(1);
    vector<double> res = calc.calc(length,len,input[0],input[1],output[0],output[1],output[2]);
    ui->lineEdit_3->setText(QString::number(res[0]));
    ui->lineEdit_4->setText(QString::number(res[1]));
    ui->lineEdit_5->setText(QString::number(res[2]));
}

void MainWindow::on_pushButton_clicked()
{
    calculate();
}
