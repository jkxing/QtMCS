#ifndef PREDICTDIALOG_H
#define PREDICTDIALOG_H

#include <QDialog>

namespace Ui {
class PredictDialog;
}

class PredictDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PredictDialog(QWidget *parent = 0);
    ~PredictDialog();
    void work();
private:
    Ui::PredictDialog *ui;
signals:
    void finish(int,double,double,double,int,int,int,int,int);
};

#endif // PREDICTDIALOG_H
