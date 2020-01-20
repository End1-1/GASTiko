#ifndef WORKING_H
#define WORKING_H

#include <QDialog>

namespace Ui {
class Working;
}

class MainDialog;

class Working : public QDialog
{
    Q_OBJECT

public:
    explicit Working(QWidget *parent = 0);

    ~Working();

    static void openWorking(MainDialog *d);

private slots:
    void on_btnCancel_clicked();

    void on_btnWrite_clicked();

    void on_leCard_returnPressed();

    void on_leAmount_textChanged(const QString &arg1);

    void on_chUseBonus_clicked(bool checked);

private:
    Ui::Working *ui;

    MainDialog *fMainDialog;

    void setFinalPayment();
};

#endif // WORKING_H
