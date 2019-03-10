#include "datedialog.h"
#include "ui_datedialog.h"
#include "config.h"

DateDialog::DateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateDialog)
{
    ui->setupUi(this);
    ui->de1->setDate(QDate::currentDate());
    ui->de2->setDate(QDate::currentDate());
}

DateDialog::~DateDialog()
{
    delete ui;
}

bool DateDialog::getDates(QDate &d1, QDate &d2)
{
    DateDialog *d = new DateDialog(Config::parentWidget());
    bool result = d->exec() == QDialog::Accepted;
    if (result) {
        d1 = d->ui->de1->date();
        d2 = d->ui->de2->date();
    }
    delete d;
    return result;
}

void DateDialog::on_btnCancel_clicked()
{
    reject();
}

void DateDialog::on_btnAccept_clicked()
{
    accept();
}
