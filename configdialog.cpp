#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    for(int i=5;i<9;i++)
    {
        ui->length->addItem(QString::number(i));
    }
    on_length_currentTextChanged("5");
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}



void ConfigDialog::on_length_currentTextChanged(const QString &arg1)
{
    int num=arg1.toInt();
    ui->Input1->clear();
    ui->Input2->clear();
    ui->Output1->clear();
    ui->Output2->clear();
    ui->Output3->clear();
    for(int i=0;i<num;i++)
    {
        ui->Input1->addItem(QString::number(i));
        ui->Input2->addItem(QString::number(i));
        ui->Output1->addItem(QString::number(i));
        ui->Output2->addItem(QString::number(i));
        ui->Output3->addItem(QString::number(i));
    }
}

void ConfigDialog::work(){
    if(this->exec()==QDialog::Accepted)
    {
        emit finish(ui->length->currentText().toInt(),
                    ui->Input1->currentText().toInt(),
                    ui->Input2->currentText().toInt(),
                    ui->Output1->currentText().toInt(),
                    ui->Output2->currentText().toInt(),
                    ui->Output3->currentText().toInt()
                    );
    }
    else
    {
        emit finish(0,0,0,0,0,0);
        return;
    }
}

