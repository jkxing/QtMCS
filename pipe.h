#ifndef PIPE_H
#define PIPE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QStylePainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

class Pipe: public QGraphicsItem
{
   // Q_OBJECT
    bool inUse;
    double x,y,w,h;
public:
    Pipe(double x,double y,double w,double h,int c,QWidget* parent=0);

    QRectF boundingRect() const;

    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget);

    bool getState();
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
};
#endif // PIPE_H
