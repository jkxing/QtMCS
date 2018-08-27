#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    for(int i=5;i<9;i++)
    {
        ui->height->addItem(QString::number(i));
        ui->weight->addItem(QString::number(i));
    }
    on_weight_currentTextChanged("5");
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}



void ConfigDialog::on_weight_currentTextChanged(const QString &arg1)
{
    int num=arg1.toInt();
    ui->input1->clear();
    ui->input2->clear();
    ui->input3->clear();
    ui->output1->clear();
    ui->output1->clear();
    for(int i=1;i<=num;i++)
    {
        ui->input1->addItem(QString::number(i));
        ui->input2->addItem(QString::number(i));
        ui->input3->addItem(QString::number(i));
        ui->output1->addItem(QString::number(i));
        ui->output2->addItem(QString::number(i));
    }
}

void ConfigDialog::work(){
    if(this->exec()==QDialog::Accepted)
    {
        emit finish(ui->height->currentText().toInt(),ui->weight->currentText().toInt());
    }
    else
    {
        emit finish(0,0);
        return;
    }
}

