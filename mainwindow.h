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
public:
    int length;
    int input[2];
    int output[3];
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConfig_triggered();
    void resize(int,int,int,int,int,int);
    void on_pushButton_clicked();

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
