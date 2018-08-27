#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <configdialog.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConfig_triggered()
{
    ConfigDialog configDialog(this);
    connect(&configDialog,SIGNAL(finish(int,int)),this,SLOT(resize(int,int)));
    configDialog.work();
}
void MainWindow::resize(int a,int b)
{
    height=a;
    weight=b;
    qDebug()<<height<<" "<<weight;
}

