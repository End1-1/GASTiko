#ifndef DISCOUNTCARD_H
#define DISCOUNTCARD_H

#include <QDialog>

namespace Ui {
class DiscountCard;
}

class DiscountCard : public QDialog
{
    Q_OBJECT

public:
    explicit DiscountCard(QWidget *parent = nullptr);
    ~DiscountCard();
    void setId(const QString &id);

private slots:
    void on_btnWrite_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DiscountCard *ui;
};

#endif // DISCOUNTCARD_H
