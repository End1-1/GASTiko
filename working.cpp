#include "working.h"
#include "ui_working.h"
#include "maindialog.h"
#include "database.h"
#include "config.h"
#include <QMessageBox>
#include <QDate>

Working::Working(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Working)
{
    ui->setupUi(this);
    ui->leName->setEnabled(false);
    ui->lbCaption->setVisible(false);
    on_chUseBonus_clicked(false);

}

Working::~Working()
{
    delete ui;
}

void Working::openWorking(MainDialog *d)
{
    Working *w = new Working(d);
    w->fMainDialog = d;
    w->exec();
    delete w;
}

void Working::on_btnCancel_clicked()
{
    reject();
}

void Working::on_btnWrite_clicked()
{
    if (ui->leCard->text().isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("The card cannot be empty"));
        return;
    }
    Database db;
    if (ui->lbCaption->isVisible()) {
        ui->leName->setText(ui->leName->text().trimmed());
        if (ui->leName->text().isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("The name cannot be empty"));
            return;
        }
        if (!db.prepare("insert into cards values (:fid, :fcostumer, current_date)")) {
            QMessageBox::critical(this, tr("Database error"), db.fLastError);
            return;
        }
        db[":fid"] = ui->leCard->text();
        db[":fcostumer"] = ui->leName->text();
        if (!db.exec()) {
            QMessageBox::critical(this, tr("Database error"), db.fLastError);
            return;
        }
    }
    if (!db.prepare("insert into history values (null, :fcard, :fdate, :famount, :fscale, :fbonus)")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    double bonus = ui->leCurrentBonus->text().toDouble();

    db[":fcard"] = ui->leCard->text();
    db[":fdate"] = QDate::currentDate();
    db[":famount"] = ui->leAmount->text().toDouble();
    db[":fscale"] = Config::scaleFactor();
    db[":fbonus"] = bonus;
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    fMainDialog->addLastRow(db);

    if (ui->chUseBonus->isChecked()) {
        on_leCard_returnPressed();
        bonus = ui->leBonus->text().toDouble();
        if (bonus > ui->leAmount->text().toDouble()) {
            bonus = ui->leAmount->text().toDouble();
        }
        db[":fcard"] = ui->leCard->text();
        db[":fdate"] = QDate::currentDate();
        db[":famount"] = 0;
        db[":fscale"] = 0;
        db[":fbonus"] = bonus * -1;
        if (!db.exec()) {
            QMessageBox::critical(this, tr("Database error"), db.fLastError);
            return;
        }
        fMainDialog->addLastRow(db);
    }

    ui->lbCaption->setVisible(false);
    ui->leName->setEnabled(false);
    ui->leName->clear();
    ui->leAmount->clear();
    ui->leBonus->clear();
    ui->leCurrentBonus->clear();
    ui->leCard->clear();
    ui->leCard->setFocus();
}

void Working::on_leCard_returnPressed()
{
    ui->leCard->setText(ui->leCard->text().replace(";", "").replace("?", ""));
    Database db;
    if (!db.prepare("select fcostumer from cards where fid=:fid")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    db[":fid"] = ui->leCard->text();
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    if (db.next()) {
        ui->leName->setText(db.getString(0));
        ui->leName->setEnabled(false);
        ui->leAmount->setFocus();
        db.prepare("select sum(fbonus) from history where fcard=:fcard");
        db[":fcard"] = ui->leCard->text();
        db.exec();
        db.next();
        ui->leBonus->setText(db.getString(0));
    } else {
        ui->leName->setEnabled(true);
        ui->lbCaption->setVisible(true);
        ui->leName->setFocus();
    }
}

void Working::on_leAmount_textChanged(const QString &arg1)
{
    ui->leCurrentBonus->setText(QString::number(arg1.toDouble() * Config::scaleFactor(), 'f', 2));
    setFinalPayment();
}

void Working::on_chUseBonus_clicked(bool checked)
{
    ui->leAmoutToPay->setVisible(checked);
    ui->lbAmountToPay->setVisible(checked);
    setFinalPayment();
}

void Working::setFinalPayment()
{
    if (ui->chUseBonus->isChecked()) {
        ui->leAmoutToPay->setText(QString::number(ui->leAmount->text().toDouble() - ui->leCurrentBonus->text().toDouble() - ui->leBonus->text().toDouble(), 'f', 2));
        if (ui->leAmoutToPay->text().toDouble() < 0) {
            ui->leAmoutToPay->setText("0");
        }
    }
}
