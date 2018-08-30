#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(int len,int in1,int in2,int out1,int out2,int out3,QWidget *parent = 0);
    ~ConfigDialog();
    void work();
private slots:
    void on_length_currentTextChanged(const QString &arg1);

signals:
    void finish(int,int,int,int,int,int);
private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
