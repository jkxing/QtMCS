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
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    void work();
private slots:
    void on_weight_currentTextChanged(const QString &arg1);

signals:
    void finish(int,int);
private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
