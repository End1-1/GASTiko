#ifndef DATEDIALOG_H
#define DATEDIALOG_H

#include <QDialog>

namespace Ui {
class DateDialog;
}

class DateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateDialog(QWidget *parent = 0);

    ~DateDialog();

    static bool getDates(QDate &d1, QDate &d2);

private slots:
    void on_btnCancel_clicked();

    void on_btnAccept_clicked();

private:
    Ui::DateDialog *ui;
};

#endif // DATEDIALOG_H
