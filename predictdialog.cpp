#include "predictdialog.h"
#include "ui_predictdialog.h"
#include <QMessageBox>
PredictDialog::PredictDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PredictDialog)
{
    ui->setupUi(this);
    for(int i=5;i<9;i++)
        ui->comboBox->addItem(QString::number(i));
}

PredictDialog::~PredictDialog()
{
    delete ui;
}

void PredictDialog::work()
{
    if(this->exec()==QDialog::Accepted)
    {
        int len = ui->comboBox->currentText().toInt();
        double output1 = ui->lineEdit_2->text().toInt();
        double output2 = ui->lineEdit_3->text().toInt();
        double output3 = ui->lineEdit_4->text().toInt();
        int cnt=0,in[8],out[8];
        bool valid = true;
        if(ui->checkBox->isChecked()) in[cnt++]=0;
        if(ui->checkBox_2->isChecked()) in[cnt++]=1;
        if(ui->checkBox_3->isChecked()) in[cnt++]=2;
        if(ui->checkBox_4->isChecked()) in[cnt++]=3;
        if(ui->checkBox_5->isChecked()) in[cnt++]=4;
        if(ui->checkBox_6->isChecked()) in[cnt++]=5;
        if(ui->checkBox_7->isChecked()) in[cnt++]=6;
        if(ui->checkBox_8->isChecked()) in[cnt++]=7;
        if(cnt!=2||in[0]>=in[1]||in[1]>=len) valid = false;
        cnt=0;
        if(ui->checkBox_9->isChecked()) out[cnt++]=0;
        if(ui->checkBox_10->isChecked()) out[cnt++]=1;
        if(ui->checkBox_11->isChecked()) out[cnt++]=2;
        if(ui->checkBox_12->isChecked()) out[cnt++]=3;
        if(ui->checkBox_13->isChecked()) out[cnt++]=4;
        if(ui->checkBox_14->isChecked()) out[cnt++]=5;
        if(ui->checkBox_15->isChecked()) out[cnt++]=6;
        if(ui->checkBox_16->isChecked()) out[cnt++]=7;
        if(cnt!=3||out[0]>=out[1]||out[1]>=out[2]||out[2]>=len) valid = false;
        if(valid)
            emit finish(len,output1,output2,output3,in[0],in[1],out[0],out[1],out[2]);
        else
        {
            QMessageBox::warning(this,"Warning","Invalid Argument");
            work();
        }
    }
    else emit finish(0,0,0,0,0,0,0,0,0);
}
