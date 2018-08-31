#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <configdialog.h>
#include <QDebug>
#include <predictdialog.h>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->gridLayoutWidget);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->lineEdit->setText(QString::number(200));
    ui->lineEdit_2->setText(QString::number(200));
    ui->lineEdit_3->setEnabled(0);
    ui->lineEdit_4->setEnabled(0);
    ui->lineEdit_5->setEnabled(0);
    ui->horizontalSlider->setMinimum(20);
    ui->horizontalSlider->setMaximum(2000);
    setFixedSize(800,600);
    input[0]=0;
    input[1]=1;
    output[0]=0;
    output[1]=1;
    output[2]=2;
    length=5;
    setWindowTitle("MCS");
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
    if(len==0)
        return;
    length=len;
    input[0]=in1;
    input[1]=in2;
    output[0]=out1;
    output[1]=out2;
    output[2]=out3;
    double x=80;
    double y=10;
    double xpy=x+y;
    setFixedSize(length*xpy+300,length*xpy+300);
    //delete scene;
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
double MainWindow::calcLoss(double x,double y,double z,double a,double b,double c)
{
    if(isnan(x)||isnan(y)||isnan(z)) return 1000000000;
    return (x-a)*(x-a)+(y-b)*(y-b)+(z-c)*(z-c);
}
void MainWindow::predict(int _length,double output1,double output2,double output3,int in1,int in2,int out1,int out2,int out3)
{
    if(_length==0) return;
    length = _length;
    input[0]=in1;
    input[1]=in2;
    output[0]=out1;
    output[1]=out2;
    output[2]=out3;
    vector<double> len;
    for(int i=0;i<2*length*length-2*length;i++)
    {
        int k=rand()%10;
        if(k)
            len.push_back(1);
        else
            len.push_back(0);
    }
    for(int i=0;i<5;i++)
        len.push_back(1);
    int siz = 2*length*(length-1)+5;
    double inflow = (output1+output2+output3)/2;
    vector<double> res = calc.calc(length,len,in1,in2,out1,out2,out3,inflow,inflow);
    vector<double> bst = res;
    double loss = calcLoss(res[siz-3],res[siz-2],res[siz-1],output1,output2,output3);
    qDebug()<<loss<<endl;
    for(int tim=0;tim<1000;tim++)
    {
        int k = rand()%(siz-5);
        if(len[k]>0.5) len[k]=0;
        else len[k]=1;
        res = calc.calc(length,len,in1,in2,out1,out2,out3,inflow,inflow);
        double loss1 = calcLoss(res[siz-3],res[siz-2],res[siz-1],output1,output2,output3);
        if(loss<loss1)
        {
            if(len[k]>0.5) len[k]=0;
            else len[k]=1;
        }
        else
        {
            loss = loss1;
            bst = res;
        }
    }
    qDebug()<<loss<<endl;
    double x=80;
    double y=10;
    double xpy=x+y;
    setFixedSize(length*xpy+300,length*xpy+300);
    //delete scene;
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    double left = -x*length/2;
    double top = -x*length/2;
    int cnt=0;
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length-1;j++)
        {
            pipe[1][i][j]=new Pipe(1,i,j,left+xpy*i,top+xpy*j+y,y,x,len[cnt]);
            scene->addItem(pipe[1][i][j]);
            connect(pipe[1][i][j],SIGNAL(PipeEdit(int,int,int)),this,SLOT(EditPipe(int,int,int)));
            cnt++;
        }
    }
    for(int i=0;i<length-1;i++)
    {
        for(int j=0;j<length;j++)
        {
            pipe[0][i][j]=new Pipe(0,i,j,left+xpy*i+y,top+xpy*j,x,y,len[cnt]);
            scene->addItem(pipe[0][i][j]);
            connect(pipe[0][i][j],SIGNAL(PipeEdit(int,int,int)),this,SLOT(EditPipe(int,int,int)));
            cnt++;
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
    ui->lineEdit->setText(QString::number(inflow));
    ui->lineEdit_2->setText(QString::number(inflow));
    calculate();
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
                len.push_back(pipe[1][i][j]->getWidth());
            else
                len.push_back(0);
        }
    for(int i=0;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(pipe[0][i][j]->getState())
                len.push_back(pipe[0][i][j]->getWidth());
            else
                len.push_back(0);
        }
    for(int i=0;i<2;i++)
        len.push_back(inpipe[i]->getWidth());
    for(int i=0;i<3;i++)
        len.push_back(outpipe[i]->getWidth());
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
                pipe[1][i][j]->changeSpeed(res[cnt],(input1+input2));
            }
            cnt++;
        }
    for(int i=0;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(pipe[0][i][j]->getState())
            {
                pipe[0][i][j]->changeSpeed(res[cnt],(input1+input2));
            }
            cnt++;
        }
    inpipe[0]->changeSpeed(res[siz-5],(input1+input2));
    inpipe[1]->changeSpeed(res[siz-4],(input1+input2));
    outpipe[0]->changeSpeed(res[siz-3],(input1+input2));
    outpipe[1]->changeSpeed(res[siz-2],(input1+input2));
    outpipe[2]->changeSpeed(res[siz-1],(input1+input2));
    inpipe[0]->changeConcentration(inpipe[0]->getSpeed());
    inpipe[1]->changeConcentration(0);
    calcConcentration(res,input1);
}
void MainWindow::EditPipe(int id,int x,int y)
{
    current_select = 1;
    select_id = id;
    select_x = x;
    select_y = y;
    double wid;
    double spd;
    double concentration;
    if(select_id==3)
    {
        wid = inpipe[y]->getWidth();
        spd = inpipe[y]->getSpeed()/wid;
        concentration = inpipe[y]->getConcentration();
    }
    else if(select_id==4)
    {
        wid = outpipe[y]->getWidth();
        spd = outpipe[y]->getSpeed()/wid;
        concentration = outpipe[y]->getConcentration();
    }
    else
    {
        wid = pipe[id][x][y]->getWidth();
        spd = pipe[id][x][y]->getSpeed()/wid;
        concentration = pipe[id][x][y]->getConcentration();
    }
    ui->statusbar->showMessage("Selected pipe speed is "+QString::number(spd)+" nong du is " + QString::number(concentration),2000);
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
            outpipe[select_y]->changeWidth(wid);
            scene->removeItem(outpipe[select_y]);
            scene->addItem(outpipe[select_y]);
        }
        else
        {
            bool can = 1;
            if(select_id==1)
            {
                if(select_x>0&&wid+pipe[1][select_x-1][select_y]->getWidth()*200>3200)
                    can=0;
                if(select_x<length-1&&wid+pipe[1][select_x+1][select_y]->getWidth()*200>3200)
                    can=0;
            }
            else
            {
                if(select_y>0&&wid+pipe[0][select_x][select_y-1]->getWidth()*200>3200)
                    can=0;
                if(select_y<length-1&&wid+pipe[0][select_x][select_y+1]->getWidth()*200>3200)
                    can=0;
            }
            if(!can)
            {
                QMessageBox::warning(this,"Warning","Too narrow between pipes");
                return;
            }
            pipe[select_id][select_x][select_y]->changeWidth(wid);
            scene->removeItem(pipe[select_id][select_x][select_y]);
            scene->addItem(pipe[select_id][select_x][select_y]);
        }
    }
}

void MainWindow::calcConcentration(vector<double>& res,double input1)
{
    memset(deg,0,sizeof(deg));
    memset(mp,0,sizeof(mp));
    int cnt=0;
    const double eps=1e-5;
    for(int i=0;i<length;i++)
        for(int j=0;j<length-1;j++)
        {
            if(res[cnt]>eps)
            {
                deg[i][j+1]++;
                mp[i][j][i][j+1]=1;
            }
            else if(res[cnt]<-eps)
            {
                deg[i][j]++;
                mp[i][j+1][i][j]=1;
            }
            pipe[1][i][j]->changeConcentration(0);
            cnt++;
        }
    for(int i=0;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(res[cnt]>eps)
            {
                deg[i+1][j]++;
                mp[i][j][i+1][j]=1;
            }
            else if(res[cnt]<-eps)
            {
                deg[i][j]++;
                mp[i+1][j][i][j]=1;
            }
            pipe[0][i][j]->changeConcentration(0);
            cnt++;
        }
    memset(flag,0,sizeof(flag));
    for(int i=1;i<length-1;i++)
        for(int j=0;j<length;j++)
        {
            if(j==0)
            {
                if((i==input[0]||i==input[1])&&mp[i][j][i][j+1]) flag[0][i][j]=1;
            }
            else if(j==length-1)
            {
                if((i==output[0]||i==output[1]||i==output[2])&&mp[i][j-1][i][j]) flag[0][i][j]=1;
            }
            else
            {
                if(mp[i][j][i][j+1]*mp[i][j-1][i][j]) flag[0][i][j]=1;
                if(mp[i][j][i][j-1]*mp[i][j+1][i][j]) flag[0][i][j]=2;
            }
            if(mp[i][j][i+1][j]*mp[i-1][j][i][j]) flag[1][i][j]=1;
            if(mp[i][j][i-1][j]*mp[i+1][j][i][j]) flag[1][i][j]=2;
            if(flag[i][j])
            {
                qDebug()<<i<<" "<<j;
            }
        }
    memset(val,0,sizeof(val));
    val[input[0]][0]=inpipe[0]->getWidth()*input1;
    front=1;tail=0;
    /*for(int i=0;i<length;i++)
    {
        for(int j=0;j<length;j++)
            qDebug()<<i<<" "<<j<<" "<<deg[i][j]<<endl;
    }*/
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
            k+=pipe[0][x][y]->getSpeed();
        if(mp[x][y][x-1][y])
            k+=-pipe[0][x-1][y]->getSpeed();
        if(mp[x][y][x][y+1])
            k+=pipe[1][x][y]->getSpeed();
        if(mp[x][y][x][y-1])
            k+=-pipe[1][x][y-1]->getSpeed();
        if(y==length-1)
        {
            for(int i=0;i<3;i++)
                if(x==output[i])
                    k+=outpipe[i]->getSpeed();
        }
        bool spe=0;
        if(flag[0][x][y]&&flag[1][x][y])
        {
            Pipe *tin1,*tout1,*tin2,*tout2;
            if(flag[0][x][y]==1)
            {
                if(y==0)
                {
                    if(x==input[0])
                        tin1 = inpipe[0];
                    else tin1 = inpipe[1];
                    tout1 = pipe[1][x][y];
                }
                else if(y==length-1)
                {
                    tin1 = pipe[1][x][y-1];
                    for(int i=0;i<3;i++)
                        if(x==output[i])
                            tout1 = outpipe[i];
                }
                else
                {
                    tin1 = pipe[1][x][y-1];
                    tout1 = pipe[1][x][y];
                }
            }
            else
            {
                tin1 = pipe[1][x][y];
                tout1 = pipe[1][x][y-1];
            }
            if(flag[1][x][y]==1)
            {
                tin2 = pipe[0][x-1][y];
                tout2 = pipe[0][x][y];
            }
            else
            {
                tin2 = pipe[0][x][y];
                tout2 = pipe[0][x-1][y];
            }
            if(fabs(tin1->getSpeed())>fabs(tout2->getSpeed()))
            {
                qDebug()<<x<<" "<<y<<" "<<val[x][y]<<" "<<val[x][y]-fabs(tin1->getConcentration()*tout2->getSpeed());
                spe = 1;
                tout2->changeConcentration((tin1->getConcentration()*fabs(tout2->getSpeed())));
                tout1->changeConcentration((val[x][y]-fabs(tin1->getConcentration()*tout2->getSpeed())));
                if(tout1->getid()<2)
                {
                    if(tout1->getSpeed()>0.000001)
                        val[tout1->getX()][tout1->getY()+1]+=fabs(tout1->getSpeed()*tout1->getConcentration());
                    else
                        val[tout1->getX()][tout1->getY()]+=fabs(tout1->getSpeed()*tout1->getConcentration());
                }
                if(tout2->getid()<2)
                {
                    if(tout2->getSpeed()>0.000001)
                        val[tout2->getX()+1][tout2->getY()]+=fabs(tout2->getSpeed()*tout2->getConcentration());
                    else
                        val[tout2->getX()][tout2->getY()]+=fabs(tout2->getSpeed()*tout2->getConcentration());
                }
            }
            else if(fabs(tin2->getSpeed())>fabs(tout1->getSpeed()))
            {
                spe = 1;
                tout1->changeConcentration(tin2->getConcentration()*fabs(tout1->getSpeed()));
                tout2->changeConcentration((val[x][y]-tin2->getConcentration()*fabs(tout1->getSpeed())));

                if(tout1->getid()<2)
                {
                    if(tout1->getSpeed()>0.000001)
                        val[tout1->getX()][tout1->getY()+1]+=fabs(tout1->getSpeed()*tout1->getConcentration());
                    else
                        val[tout1->getX()][tout1->getY()]+=fabs(tout1->getSpeed()*tout1->getConcentration());
                }
                if(tout2->getid()<2)
                {
                    if(tout2->getSpeed()>0.000001)
                        val[tout2->getX()+1][tout2->getY()]+=fabs(tout2->getSpeed()*tout2->getConcentration());
                    else
                        val[tout2->getX()][tout2->getY()]+=fabs(tout2->getSpeed()*tout2->getConcentration());
                }
            }
        }
        if(mp[x][y][x+1][y])
        {
            if((--deg[x+1][y])==0)
            {
                tail++;
                qx[tail]=x+1;
                qy[tail]=y;
            }
            if(!spe)
            {
                pipe[0][x][y]->changeConcentration(val[x][y]*pipe[0][x][y]->getSpeed()/k);
                val[x+1][y]+=val[x][y]*pipe[0][x][y]->getSpeed()/k;
            }
        }
        if(mp[x][y][x-1][y])
        {
            if((--deg[x-1][y])==0)
            {
                tail++;
                qx[tail]=x-1;
                qy[tail]=y;
            }
            if(!spe)
            {
                pipe[0][x-1][y]->changeConcentration(val[x][y]*-1*pipe[0][x-1][y]->getSpeed()/k);
                val[x-1][y]+=val[x][y]*-pipe[0][x-1][y]->getSpeed()/k;

            }
        }
        if(mp[x][y][x][y+1])
        {
            if((--deg[x][y+1])==0)
            {
                tail++;
                qx[tail]=x;
                qy[tail]=y+1;
            }
            if(!spe)
            {
                pipe[1][x][y]->changeConcentration(val[x][y]*pipe[1][x][y]->getSpeed()/k);
                val[x][y+1]+=val[x][y]*pipe[1][x][y]->getSpeed()/k;
            }
        }
        if(mp[x][y][x][y-1])
        {
            if((--deg[x][y-1])==0)
            {
                tail++;
                qx[tail]=x;
                qy[tail]=y-1;
            }
            if(!spe)
            {
                pipe[1][x][y-1]->changeConcentration(val[x][y]*-pipe[1][x][y-1]->getSpeed()/k);
                val[x][y-1]+=val[x][y]*-pipe[1][x][y-1]->getSpeed()/k;
            }
        }
        if(y==length-1)
        {
            if(!spe)
            {
                for(int i=0;i<3;i++)
                    if(x==output[i])
                        outpipe[i]->changeConcentration(val[x][y]*outpipe[i]->getSpeed()/k);
            }
        }
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->pushButton_2->setText(QString::number(value)+"(click to modify)");
}

void MainWindow::on_actionPredict_triggered()
{
    PredictDialog pd(this);
    connect(&pd,SIGNAL(finish(int,double,double,double,int,int,int,int,int)),this,SLOT(predict(int,double,double,double,int,int,int,int,int)));
    pd.work();
}
