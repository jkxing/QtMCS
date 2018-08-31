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
    double width;
    double speed;
    int pos,_x,_y;
    QColor col;
    double concentration;
public:
    Pipe(int pos,int _x,int _y,double x,double y,double w,double h,int c,QWidget* parent=0);

    QRectF boundingRect() const;

    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option
, QWidget *widget);

    bool getState();
    double getWidth();
    double getSpeed();
    void changeSpeed(double x,double input);
    void changeWidth(double x);
    void changeConcentration(double x);
    double getConcentration();
    int getid();
    int getX();
    int getY();
signals:
    void PipeEdit(int,int,int);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};
#endif // PIPE_H
