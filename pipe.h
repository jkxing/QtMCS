#ifndef PIPE_H
#define PIPE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QStylePainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

class Pipe: public QGraphicsObject
{
    Q_OBJECT
    bool inUse;
    double x,y,w,h;
    double chang,kuan;
    double speed;
    int pos,_x,_y;
    QColor col;
    double nongdu;
public:
    Pipe(int pos,int _x,int _y,double x,double y,double w,double h,int c,QWidget* parent=0);

    QRectF boundingRect() const;

    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget);

    bool getState();
    double getChang();
    double getKuan();
    void setSpeed(double x);
    double getSpeed();
    void changeSpeed(double x);
    void changeWidth(double x);
    void changeNongDu(double x);
    double getNongDu();
signals:
    void PipeEdit(int,int,int);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};
#endif // PIPE_H
