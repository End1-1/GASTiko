#include "configdialog.h"
#include "ui_configdialog.h"
#include "config.h"
#include "maindialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    ui->leScaleFactor->setText(QString::number(Config::scaleFactor(), 'f', 2));
    ui->leSlogan->setText(Config::slogan());
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::openDialog()
{
    ConfigDialog *d = new ConfigDialog(Config::parentWidget());
    d->exec();
    delete d;
}

void ConfigDialog::on_btnCancel_clicked()
{
    reject();
}

void ConfigDialog::on_btnSave_clicked()
{
    Config::setScaleFactor(ui->leScaleFactor->text().toDouble());
    Config::setSlogan(ui->leSlogan->text());
    static_cast<MainDialog*>(Config::parentWidget())->setSlogan(ui->leSlogan->text());
    accept();
}
