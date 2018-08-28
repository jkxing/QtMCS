#include "pipe.h"

Pipe::Pipe(double x,double y,double w,double h,int c,QWidget* parent):x(x),y(y),w(w),h(h)
{
    if(c==-1)
    {
        inUse = rand()%10?1:0;
    }
    else
    {
        inUse = c;
    }
    //setFlag(ItemIsMovable);
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
        brush.setColor(Qt::red);
    }
    else
    {
        brush.setColor(Qt::gray);
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

bool Pipe::getState(){
    return inUse;
}
