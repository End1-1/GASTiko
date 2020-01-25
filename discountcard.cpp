#include "discountcard.h"
#include "ui_discountcard.h"
#include "database.h"
#include <QMessageBox>

DiscountCard::DiscountCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiscountCard)
{
    ui->setupUi(this);
}

DiscountCard::~DiscountCard()
{
    delete ui;
}

void DiscountCard::setId(const QString &id)
{
    ui->lbCaption->setVisible(false);
    ui->leCard->setText(id);
    Database db;
    db.prepare("select fcostumer, fphone, fvalue from discountcards where fid=:fid");
    db[":fid"] = id;
    db.exec();
    if (db.next()) {
        ui->leName->setText(db.getString(0));
        ui->lePhone->setText(db.getString(1));
        ui->leBonus->setText(db.getString(2));
    } else {
        accept();
    }
}

void DiscountCard::on_btnWrite_clicked()
{
    Database db;
    ui->leName->setText(ui->leName->text().trimmed());
    if (ui->leName->text().isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("The name cannot be empty"));
        return;
    }
    if (ui->lbCaption->isVisible()) {
        if (!db.prepare("insert into DISCOUNTCARDS (fid, fdate, fcostumer, fphone, fvalue) values (:fid, current_date, :fcostumer, :fphone, :fvalue)")) {
            QMessageBox::critical(this, tr("Database error"), db.fLastError);
            return;
        }
    } else {
        if (!db.prepare("update DISCOUNTCARDS set fcostumer=:fcostumer, fphone=:fphone, fvalue=:fvalue where fid=:fid")) {
            QMessageBox::critical(this, tr("Database error"), db.fLastError);
            return;
        }
    }
    db[":fid"] = ui->leCard->text();
    db[":fcostumer"] = ui->leName->text();
    db[":fphone"] = ui->lePhone->text();
    db[":fvalue"] = ui->leBonus->text().toDouble();
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    accept();
}

void DiscountCard::on_btnCancel_clicked()
{
    reject();
}
