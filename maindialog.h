#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include "database.h"
#include <QDialog>

namespace Ui {
class MainDialog;
}

enum ReportMode {rmWorking = 0, rmCards, rmDiscountCards};

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);

    ~MainDialog();

    void addLastRow(Database &db);

    void setSlogan(const QString &text);

private slots:
    void deleteHistoryRow();

    void deleteCardRow();

    void newDiscountCard();

    void editDiscountCard();

    void deleteDiscountCard();

    void on_btnWorking_clicked();

    void on_btnCards_clicked();

    void on_btnReportByDate_clicked();

    void on_btnConfig_clicked();

    void on_tbl_customContextMenuRequested(const QPoint &pos);

    void on_btnDiscountCards_clicked();

private:
    Ui::MainDialog *ui;

    void reportWorking(const QDate &d1, const QDate &d2);

    void setSum(QList<int> cols);

    ReportMode fReportMode;
};

#endif // MAINDIALOG_H
