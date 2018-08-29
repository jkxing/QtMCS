#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <configdialog.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->gridLayoutWidget);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->lineEdit_3->setEnabled(0);
    ui->lineEdit_4->setEnabled(0);
    ui->lineEdit_5->setEnabled(0);
    ui->horizontalSlider->setMinimum(20);
    ui->horizontalSlider->setMaximum(1000);
    setFixedSize(800,600);
    input[0]=0;
    input[1]=1;
    output[0]=0;
    output[1]=1;
    output[2]=2;
    length=5;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConfig_triggered()
{
    ConfigDialog configDialog(length,input[0],input[1],output[0],output[1],output[2],this);
    connect(&configDialog,SIGNAL(finish(int,int,int,int,int,int)),this,SLOT(resize(int,int,int,int,int,int)));
    configDialog.work();
}
void MainWindow::resize(int len,int in1,int in2,int out1,int out2,int out3)
{
    length=len;
    input[0]=in1;
    input[1]=in2;
    output[0]=out1;
    output[1]=out2;
    output[2]=out3;
    double x=120;
    double y=15;
    double xpy=x+y;
    setFixedSize(length*xpy+500,length*xpy+500);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    double left = -x*length/2;
    double top = -x*length/2;
    for(int i=0;i<length-1;i++)
    {
        for(int j=0;j<length;j++)
        {
            pipe[0][i][j]=new Pipe(0,i,j,left+xpy*i+y,top+xpy*j,x,y,-1);
            scene->addItem(pipe[0][i][j]);
            connect(pipe[0][i][j],SIGNAL(PipeEdit(int,int,int)),this,SLOT(EditPipe(int,int,int)));
        }
    }
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length-1;j++)
        {
            pipe[1][i][j]=new Pipe(1,i,j,left+xpy*i,top+xpy*j+y,y,x,-1);
            scene->addItem(pipe[1][i][j]);
            connect(pipe[1][i][j],SIGNAL(PipeEdit(int,int,int)),this,SLOT(EditPipe(int,int,int)));
        }
    }
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length;j++)
        {
            pipe[2][i][j]=new Pipe(2,i,j,left+xpy*i,top+xpy*j,y,y,1);
            scene->addItem(pipe[2][i][j]);
        }
    }
    inpipe[0]=new Pipe(3,0,0,left+in1*xpy,top-x,y,x,1);
    inpipe[1]=new Pipe(3,0,1,left+in2*xpy,top-x,y,x,1);
    outpipe[0]=new Pipe(4,0,0,left+out1*xpy,top+xpy*length-xpy+y,y,x,1);
    outpipe[1]=new Pipe(4,0,1,left+out2*xpy,top+xpy*length-xpy+y,y,x,1);
    outpipe[2]=new Pipe(4,0,2,left+out3*xpy,top+xpy*length-xpy+y,y,x,1);
    for(int i=0;i<2;i++)
    {
        scene->addItem(inpipe[i]);
        connect(inpipe[i],SIGNAL(PipeEdit(int,int,int)),this,SLOT(EditPipe(int,int,int)));
    }
    for(int i=0;i<3;i++)
    {
        scene->addItem(outpipe[i]);
        connect(outpipe[i],SIGNAL(PipeEdit(int,int,int)),this,SLOT(EditPipe(int,int,int)));
    }
}

void MainWindow::calculate()
{
    double input1 = ui->lineEdit->text().toDouble();
    double input2 = ui->lineEdit_2->text().toDouble();
    vector<double> len;
    len.clear();
    for(int i=0;i<length;i++)
        for(int j=0;j<length-1;j++)
        {
            if(pipe[1][i][j]->getState())
                len.push_back(pipe[1][i][j]->getKuan());
            else
                len.push_back(0);
        }
    for(int i=0;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(pipe[0][i][j]->getState())
                len.push_back(pipe[0][i][j]->getKuan());
            else
                len.push_back(0);
        }
    for(int i=0;i<2;i++)
        len.push_back(inpipe[i]->getKuan());
    for(int i=0;i<3;i++)
        len.push_back(outpipe[i]->getKuan());
    vector<double> res = calc.calc(length,len,input[0],input[1],output[0],output[1],output[2],input1,input2);
    int siz = res.size();
    ui->lineEdit_3->setText(QString::number(res[siz-3]));
    ui->lineEdit_4->setText(QString::number(res[siz-2]));
    ui->lineEdit_5->setText(QString::number(res[siz-1]));
    int cnt=0;
    for(int i=0;i<length;i++)
        for(int j=0;j<length-1;j++)
        {
            if(pipe[1][i][j]->getState())
            {
                pipe[1][i][j]->setSpeed(res[cnt]);
                pipe[1][i][j]->changeSpeed(res[cnt]/(input1+input2));
            }
            else
            {
                pipe[1][i][j]->setSpeed(0);
            }
            cnt++;
        }
    for(int i=0;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(pipe[0][i][j]->getState())
            {
                pipe[0][i][j]->setSpeed(res[cnt]);
                pipe[0][i][j]->changeSpeed(res[cnt]/(input1+input2));
            }
            else
            {
                pipe[0][i][j]->setSpeed(0);
            }
            cnt++;
        }
    inpipe[0]->setSpeed(res[siz-5]);
    inpipe[1]->setSpeed(res[siz-4]);
    inpipe[0]->changeSpeed(res[siz-5]/(input1+input2));
    inpipe[1]->changeSpeed(res[siz-4]/(input1+input2));
    outpipe[0]->setSpeed(res[siz-3]);
    outpipe[1]->setSpeed(res[siz-2]);
    outpipe[2]->setSpeed(res[siz-1]);
    outpipe[0]->changeSpeed(res[siz-3]/(input1+input2));
    outpipe[1]->changeSpeed(res[siz-2]/(input1+input2));
    outpipe[2]->changeSpeed(res[siz-1]/(input1+input2));
    memset(deg,0,sizeof(deg));
    memset(mp,0,sizeof(mp));
    cnt=0;
    for(int i=0;i<length;i++)
        for(int j=0;j<length-1;j++)
        {
            if(res[cnt]>0.00001)
            {
                deg[i][j+1]++;
                mp[i][j][i][j+1]=1;
            }
            else if(res[cnt]<-0.00001)
            {
                deg[i][j]++;
                mp[i][j+1][i][j]=1;
            }
            cnt++;
            pipe[1][i][j]->changeNongDu(0);
        }
    for(int i=0;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(res[cnt]>0.00001)
            {
                deg[i+1][j]++;
                mp[i][j][i+1][j]=1;
            }
            else if(res[cnt]<-0.00001)
            {
                deg[i][j]++;
                mp[i+1][j][i][j]=1;
            }
            pipe[0][i][j]->changeNongDu(0);
            cnt++;
        }
    memset(val,0,sizeof(val));
    val[input[0]][0]=inpipe[0]->getKuan()*input1;
    front=1;tail=0;
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length;j++)
            qDebug()<<i<<" "<<j<<" "<<deg[i][j]<<endl;
    }
    for(int i=0;i<length;i++)
        for(int j=0;j<length;j++)
        if(deg[i][j]==0)
        {
            qx[++tail]=i;
            qy[tail]=j;
        }
    while(front<=tail)
    {
        int x=qx[front];
        int y=qy[front];
        front++;
        double k=0;
        if(mp[x][y][x+1][y])
        {
            deg[x+1][y]--;
            k+=pipe[0][x][y]->getSpeed()*pipe[0][x][y]->getKuan();
        }
        if(mp[x][y][x-1][y])
        {
            deg[x-1][y]--;
            k+=-pipe[0][x-1][y]->getSpeed()*pipe[0][x-1][y]->getKuan();
        }
        if(mp[x][y][x][y+1])
        {
            deg[x][y+1]--;
            k+=pipe[1][x][y]->getSpeed()*pipe[1][x][y]->getKuan();
        }
        if(mp[x][y][x][y-1])
        {
            deg[x][y-1]--;
            k+=-pipe[1][x][y-1]->getSpeed()*pipe[1][x][y-1]->getKuan();
        }

        if(y==length-1)
        {
            for(int i=0;i<3;i++)
                if(x==output[i])
                    k+=outpipe[i]->getSpeed()*outpipe[i]->getKuan();
        }
        if(mp[x][y][x+1][y])
        {
            if(deg[x+1][y]==0)
            {
                tail++;
                qx[tail]=x+1;
                qy[tail]=y;
            }
            pipe[0][x][y]->changeNongDu(val[x][y]*pipe[0][x][y]->getSpeed()*pipe[0][x][y]->getKuan()/k);
            val[x+1][y]+=val[x][y]*pipe[0][x][y]->getSpeed()*pipe[0][x][y]->getKuan()/k;
        }
        if(mp[x][y][x-1][y])
        {
            if(deg[x-1][y]==0)
            {
                tail++;
                qx[tail]=x-1;
                qy[tail]=y;
            }
            pipe[0][x-1][y]->changeNongDu(val[x][y]*-1*pipe[0][x-1][y]->getSpeed()*pipe[0][x-1][y]->getKuan()/k);
            val[x-1][y]+=val[x][y]*-pipe[0][x-1][y]->getSpeed()*pipe[0][x-1][y]->getKuan()/k;
        }
        if(mp[x][y][x][y+1])
        {
            if(deg[x][y+1]==0)
            {
                tail++;
                qx[tail]=x;
                qy[tail]=y+1;
            }
            pipe[1][x][y]->changeNongDu(val[x][y]*pipe[1][x][y]->getSpeed()*pipe[1][x][y]->getKuan()/k);
            val[x][y+1]+=val[x][y]*pipe[1][x][y]->getSpeed()*pipe[1][x][y]->getKuan()/k;
        }
        if(mp[x][y][x][y-1])
        {
            if(deg[x][y-1]==0)
            {
                tail++;
                qx[tail]=x;
                qy[tail]=y-1;
            }
            pipe[1][x][y-1]->changeNongDu(val[x][y]*-pipe[1][x][y-1]->getSpeed()*pipe[1][x][y-1]->getKuan()/k);
            val[x][y-1]+=val[x][y]*-pipe[1][x][y-1]->getSpeed()*pipe[1][x][y-1]->getKuan()/k;
        }
        if(y==length-1)
        {
            for(int i=0;i<3;i++)
                if(x==output[i])
                    outpipe[i]->changeNongDu(val[x][y]*outpipe[i]->getSpeed()*outpipe[i]->getKuan()/k);
        }
    }

}
void MainWindow::EditPipe(int id,int x,int y)
{
    current_select = 1;
    select_id = id;
    select_x = x;
    select_y = y;
    double wid;
    double spd;
    double nongdu;
    if(select_id==3)
    {
        wid = inpipe[y]->getKuan();
        spd = inpipe[y]->getSpeed();
        nongdu = inpipe[y]->getNongDu();
    }
    else if(select_id==4)
    {
        wid = outpipe[y]->getKuan();
        spd = outpipe[y]->getSpeed();
        nongdu = outpipe[y]->getNongDu();
    }
    else
    {
        wid = pipe[id][x][y]->getKuan();
        spd = pipe[id][x][y]->getSpeed();
        nongdu = pipe[id][x][y]->getNongDu();
    }
    ui->statusbar->showMessage("Selected pipe speed is "+QString::number(spd)+" nong du is " + QString::number(nongdu),2000);
    ui->horizontalSlider->setValue(wid*200);
}
void MainWindow::on_pushButton_clicked()
{
    calculate();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(current_select)
    {
        current_select=0;
        int wid = ui->horizontalSlider->value();
        if(select_id==3)
        {
            inpipe[select_y]->changeWidth(wid);
            scene->removeItem(inpipe[select_y]);
            scene->addItem(inpipe[select_y]);
        }
        else if(select_id==4)
        {
            qDebug()<<"ok";
            outpipe[select_y]->changeWidth(wid);
            scene->removeItem(outpipe[select_y]);
            scene->addItem(outpipe[select_y]);
        }
        else
        {
            pipe[select_id][select_x][select_y]->changeWidth(wid);
            scene->removeItem(pipe[select_id][select_x][select_y]);
            scene->addItem(pipe[select_id][select_x][select_y]);
        }
    }
}
