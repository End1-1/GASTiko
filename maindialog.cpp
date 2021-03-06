#include "maindialog.h"
#include "ui_maindialog.h"
#include "working.h"
#include "config.h"
#include "discountcard.h"
#include "datedialog.h"
#include "configdialog.h"
#include <QMessageBox>
#include <QDate>
#include <QMenu>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    Config::setParentWidget(this);
    setSlogan(Config::slogan());
    ui->tblTotal->setVisible(false);
    reportWorking(QDate::currentDate(), QDate::currentDate());
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::addLastRow(Database &db)
{
    if (!db.prepare("select max(fid) from history")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    if (!db.next()) {
        return;
    }
    int id = db.getInteger(0);
    if (!db.prepare("select h.fid, h.fdate, c.fcostumer, c.fphone, h.famount, h.fbonus from history h left join cards c on c.fid=h.fcard where h.fid=:fid")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    db[":fid"] = id;
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    if (db.next()) {
        ui->tbl->insertRow(0);
        ui->tbl->setInteger(0, 0, db.getInteger(0));
        ui->tbl->setString(0, 1, db.getDate(1).toString("dd/MM/yyyy"));
        ui->tbl->setString(0, 2, db.getString(2));
        ui->tbl->setString(0, 3, db.getString(3));
        ui->tbl->setDouble(0, 4, db.getDouble(4));
        ui->tbl->setDouble(0, 5, db.getDouble(5));
    }
    QList<int> sumCols;
    sumCols << 4 << 5;
    setSum(sumCols);
}

void MainDialog::setSlogan(const QString &text)
{
    ui->lbSlogan->setText(text);
    ui->lbSlogan->setVisible(!ui->lbSlogan->text().isEmpty());
}

void MainDialog::deleteHistoryRow()
{
    QModelIndexList ml = ui->tbl->selectionModel()->selectedRows();
    if (ml.count() == 0) {
        return;
    }
    if (QMessageBox::question(this, tr("Question"), tr("Confirm to remove"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }
    Database db;
    if (!db.prepare("delete from history where fid=:fid")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    db[":fid"] = ui->tbl->getInteger(ml.at(0).row(), 0);
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    ui->tbl->removeRow(ml.at(0).row());
    QList<int> sumCols;
    sumCols << 3 << 4;
    setSum(sumCols);
    QMessageBox::information(this, tr("Info"), tr("Deleted"));
}

void MainDialog::deleteCardRow()
{
    QModelIndexList ml = ui->tbl->selectionModel()->selectedRows();
    if (ml.count() == 0) {
        return;
    }
    if (QMessageBox::question(this, tr("Question"), tr("Confirm to remove"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }
    Database db;
    if (!db.prepare("delete from history where fcard=:fcard")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    db[":fcard"] = ui->tbl->getString(ml.at(0).row(), 0);
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    if (!db.prepare("delete from cards where fid=:fid")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    db[":fid"] = ui->tbl->getString(ml.at(0).row(), 0);
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    ui->tbl->removeRow(ml.at(0).row());
    QList<int> sumCols;
    sumCols << 3 << 4;
    setSum(sumCols);
    QMessageBox::information(this, tr("Info"), tr("Deleted"));
}

void MainDialog::newDiscountCard()
{
    DiscountCard *d = new DiscountCard(this);
    d->exec();
    delete d;
    on_btnDiscountCards_clicked();
}

void MainDialog::editDiscountCard()
{
    QModelIndexList ml = ui->tbl->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        return;
    }
    DiscountCard *d = new DiscountCard();
    d->setId(ui->tbl->item(ml.at(0).row(), 0)->data(Qt::DisplayRole).toString());
    d->exec();
    delete d;
}

void MainDialog::deleteDiscountCard()
{
    QModelIndexList ml = ui->tbl->selectionModel()->selectedIndexes();
    if (ml.count() == 0) {
        return;
    }
    if (QMessageBox::warning(this, tr("Confirmation"), tr("Confirm to remove card") + "<br>" + ui->tbl->item(ml.at(0).row(), 0)->data(Qt::DisplayRole).toString() + "/" + ui->tbl->item(ml.at(0).row(), 2)->data(Qt::DisplayRole).toString(), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }
    Database db;
    db.prepare("delete from discountcards where fid=:fid");
    db[":fid"] = ui->tbl->item(ml.at(0).row(), 0)->data(Qt::DisplayRole).toString();
    db.exec();
    ui->tbl->removeRow(ml.at(0).row());
}

void MainDialog::on_btnWorking_clicked()
{
    reportWorking(QDate::currentDate(), QDate::currentDate());
    Working::openWorking(this);
}

void MainDialog::on_btnCards_clicked()
{
    fReportMode = rmCards;
    ui->tbl->setRowCount(0);
    ui->tbl->clearContents();
    QStringList columnsHeader;
    columnsHeader << tr("Card")
                  << tr("Date")
                  << tr("Costumer")
                  << tr("Phone")
                  << tr("Count")
                  << tr("Amount")
                  << tr("Bonus");
    ui->tbl->setColumnWidths(columnsHeader.count(), 0, 120, 300, 200, 100, 100, 100);
    ui->tblTotal->setColumnWidths(columnsHeader.count(), 0, 120, 300, 200, 100, 100, 100);
    ui->tbl->setHorizontalHeaderLabels(columnsHeader);
    Database db;
    if (!db.prepare("select c.fid, c.fissue, c.fcostumer, c.fphone, count(h.fid), sum(h.famount), sum(h.fbonus) \
                    from cards c \
                    left join history h on h.fcard=c.fid \
                    group by 1, 2, 3, 4")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    while (db.next()) {
        int row = ui->tbl->addEmptyRow();
        ui->tbl->setInteger(row, 0, db.getInteger(0));
        ui->tbl->setString(row, 1, db.getDate(1).toString("dd/MM/yyyy"));
        ui->tbl->setString(row, 2, db.getString(2));
        ui->tbl->setString(row, 3, db.getString(3));
        ui->tbl->setDouble(row, 4, db.getDouble(4));
        ui->tbl->setDouble(row, 5, db.getDouble(5));
        ui->tbl->setDouble(row, 6, db.getDouble(6));
    }
    QList<int> sumCols;
    sumCols << 4 << 5 << 6;
    setSum(sumCols);
}

void MainDialog::reportWorking(const QDate &d1, const QDate &d2)
{
    fReportMode = rmWorking;
    ui->tbl->setRowCount(0);
    ui->tbl->clearContents();
    QStringList columnsHeader;
    columnsHeader << tr("ID")
                  << tr("Date")
                  << tr("Costumer")
                  << tr("Phone")
                  << tr("Amount")
                  << tr("Bonus");
    ui->tbl->setColumnWidths(columnsHeader.count(), 0, 120, 300, 150, 100, 100);
    ui->tblTotal->setColumnWidths(columnsHeader.count(), 0, 120, 300, 150, 100, 100);
    ui->tbl->setHorizontalHeaderLabels(columnsHeader);
    Database db;
    if (!db.prepare("select h.fid, h.fdate, c.fcostumer, c.fphone, h.famount, h.fbonus from history h left join cards c on c.fid=h.fcard where h.fdate between :fdate1 and :fdate2")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    db[":fdate1"] = d1;
    db[":fdate2"] = d2;
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    while (db.next()) {
        int row = ui->tbl->addEmptyRow();
        ui->tbl->setInteger(row, 0, db.getInteger(0));
        ui->tbl->setString(row, 1, db.getDate(1).toString("dd/MM/yyyy"));
        ui->tbl->setString(row, 2, db.getString(2));
        ui->tbl->setString(row, 3, db.getString(3));
        ui->tbl->setDouble(row, 4, db.getDouble(4));
        ui->tbl->setDouble(row, 5, db.getDouble(5));
    }
    ui->tblTotal->clear();
    ui->tblTotal->setRowCount(1);
    ui->tblTotal->setColumnCount(columnsHeader.count());
    for (int i = 0; i < ui->tbl->columnCount(); i++) {
        ui->tblTotal->setColumnWidth(i, ui->tbl->columnWidth(i));
    }
    QList<int> sumCols;
    sumCols << 4 << 5;
    setSum(sumCols);
}

void MainDialog::setSum(QList<int> cols)
{
    QList<double> vals;
    foreach (int c, cols) {
        double v = 0;
        for (int i = 0; i < ui->tbl->rowCount(); i++) {
            v += ui->tbl->getDouble(i, c);
        }
        vals << v;
    }
    int i = 0;
    foreach (int c, cols) {
        ui->tblTotal->setDouble(0, c, vals[i++]);
    }
    ui->tblTotal->setVisible(true);
}

void MainDialog::on_btnReportByDate_clicked()
{
    QDate d1, d2;
    if (DateDialog::getDates(d1, d2)) {
        reportWorking(d1, d2);
    }
}

void MainDialog::on_btnConfig_clicked()
{
    ConfigDialog::openDialog();
}

void MainDialog::on_tbl_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    switch (fReportMode) {
    case rmWorking:
        menu->addAction(tr("Delete"), this, SLOT(deleteHistoryRow()));
        break;
    case rmCards:
        menu->addAction(tr("Delete"), this, SLOT(deleteCardRow()));
        break;
    case rmDiscountCards:
        menu->addAction(tr("New"), this, SLOT(newDiscountCard()));
        menu->addAction(tr("Edit"), this, SLOT(editDiscountCard()));
        menu->addAction(tr("Delete"), this, SLOT(deleteDiscountCard()));
        break;
    default:
        break;
    }
    menu->popup(ui->tbl->mapToGlobal(pos));
}

void MainDialog::on_btnDiscountCards_clicked()
{
    fReportMode = rmDiscountCards;
    ui->tbl->setRowCount(0);
    ui->tbl->clearContents();
    QStringList columnsHeader;
    columnsHeader << tr("ID")
                  << tr("Date")
                  << tr("Costumer")
                  << tr("Phone")
                  << tr("Discount");
    ui->tbl->setColumnWidths(columnsHeader.count(), 150, 120, 300, 150, 100);
    ui->tblTotal->setColumnWidths(columnsHeader.count(), 150, 120, 300, 150, 100);
    ui->tbl->setHorizontalHeaderLabels(columnsHeader);
    Database db;
    if (!db.prepare("select fid, fdate, fcostumer, fphone, fvalue from discountcards order by fcostumer")) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    if (!db.exec()) {
        QMessageBox::critical(this, tr("Database error"), db.fLastError);
        return;
    }
    while (db.next()) {
        int row = ui->tbl->addEmptyRow();
        ui->tbl->setString(row, 0, db.getString(0));
        ui->tbl->setString(row, 1, db.getDate(1).toString("dd/MM/yyyy"));
        ui->tbl->setString(row, 2, db.getString(2));
        ui->tbl->setString(row, 3, db.getString(3));
        ui->tbl->setDouble(row, 4, db.getDouble(4));
    }
    ui->tblTotal->clear();
    ui->tblTotal->setRowCount(1);
    ui->tblTotal->setColumnCount(columnsHeader.count());
    for (int i = 0; i < ui->tbl->columnCount(); i++) {
        ui->tblTotal->setColumnWidth(i, ui->tbl->columnWidth(i));
    }
}
