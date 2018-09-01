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
    width=200;
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
void Pipe::changeSpeed(double x,double input){
    speed=x;
    if(fabs(speed)<1e-6)
    {
        col=Qt::gray;
        return;
    }
    x=fabs(speed/input)*2;
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
    update();
}
bool Pipe::getState(){
    return inUse;
}

double Pipe::getWidth(){
    return width/200;
}

double Pipe::getSpeed()
{
    return speed;
}

void Pipe::changeWidth(double t){
    double k=(t-width)*(10.0/200);
    if(pos==0)
    {
        y-=k/2;
        h+=k;
        width=t;
        update();
    }
    else
    {
        x-=k/2;
        w+=k;
        width=t;
        update();
    }
}

void Pipe::changeConcentration(double x)
{
    if(fabs(x)<0.000001) concentration=0;
    else if(fabs(getSpeed())<0.00001) concentration=0;
    else concentration=fabs(x/getSpeed());
    setOpacity(0.2+concentration*0.8);
    update();
}

double Pipe::getConcentration(){
    return concentration;
}

int Pipe::getid(){
    return pos;
}
int Pipe::getX(){
    return _x;
}
int Pipe::getY(){
    return _y;
}
