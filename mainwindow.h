#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPainter>
#include <pipe.h>
#include <vector>
#include <caluconspeed.h>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int length;
    int input[2];
    int output[3];
    int current_select,select_id,select_x,select_y;

    int deg[10][10],front,tail;
    double val[10][10];
    int qx[100],qy[100];
    int mp[10][10][10][10];
    int flag[2][10][10];
    bool needDel;
    void calcConcentration(vector<double> &res,double input1);
    double calcLoss(double x,double y,double z,double a,double b,double c);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConfig_triggered();
    void resize(int,int,int,int,int,int);
    void on_pushButton_clicked();
    void EditPipe(int,int,int);
    void on_pushButton_2_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void predict(int length,double output1,double output2,double output3,int in1,int in2,int out1,int out2,int out3);
    void on_actionPredict_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Pipe* pipe[3][8][8];
    Pipe* inpipe[2];
    Pipe* outpipe[3];
    void calculate();
    caluconspeed calc;
    vector<double> caluconspeed(int num, vector<double>&length, int i1, int i2, int o1, int o2, int o3);
};

#endif // MAINWINDOW_H
