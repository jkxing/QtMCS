#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    int height,weight;
    int input[3];
    int output[2];
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConfig_triggered();
    void resize(int,int);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
