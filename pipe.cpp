#include "pipe.h"
#include <QDebug>
Pipe::Pipe(int pos,int _x,int _y,double x,double y,double w,double h,int c,QWidget* parent):x(x),y(y),w(w),h(h),pos(pos),_x(_x),_y(_y)
{
    if(c==-1)
    {
        inUse = rand()%10?1:0;
    }
    else
    {
        inUse = c;
    }
    setFlag(ItemIsSelectable);
    kuan=200;
    chang=1600;
    col = Qt::gray;
}
QRectF Pipe::boundingRect() const
{
    return QRectF(QPointF(x,y),QPointF(x+w,y+h));
}
void Pipe::paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);

    if(inUse)
    {
        brush.setColor(col);
    }
    else
    {
        brush.setColor(Qt::white);
    }
    painter->fillRect(rec,brush);
    painter->drawRect(rec);
}
void Pipe::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    inUse ^= 1;
    update();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Pipe::mousePressEvent(QGraphicsSceneMouseEvent* event){
    emit PipeEdit(pos,_x,_y);
    QGraphicsObject::mousePressEvent(event);
}
void Pipe::changeSpeed(double x){
    x=fabs(x);
    if(x<0.16)
        col = QColor(139-x/0.16*139,0,255);
    else if(x<0.33)
        col = QColor(0,(x-0.16)/0.17*127,255);
    else if(x<0.49)
        col = QColor(0,127+(x-0.33)/0.16*128,255-(x-0.33)/0.16*255);
    else if(x<0.66)
        col = QColor((x-0.49)/0.17*255,255,0);
    else
        col = QColor(255,255-(((x>1)?1:x)-0.66)/0.34*255,0);
    //setOpacity(x+0.1);
    update();
}
bool Pipe::getState(){
    return inUse;
}

double Pipe::getChang(){
    return chang;
}

double Pipe::getKuan(){
    return kuan/200;
}

void Pipe::setSpeed(double x)
{
    speed = x;
}

double Pipe::getSpeed()
{
    return speed;
}

void Pipe::changeWidth(double t){
    double k=(t-kuan)*(15.0/200);
    if(pos==0)
    {
        y-=k/2;
        h+=k;
        kuan=t;
        update();
    }
    else
    {
        x-=k/2;
        w+=k;
        kuan=t;
        update();
    }
}

void Pipe::changeNongDu(double x)
{
    if(fabs(getSpeed())<0.00001) nongdu=0;
    else nongdu=fabs(x/getKuan()/getSpeed());
    setOpacity(0.2+nongdu*0.8);
    update();
}

double Pipe::getNongDu(){
    return nongdu;
}
