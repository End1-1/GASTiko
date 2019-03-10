#include "working.h"
#include "ui_working.h"

Working::Working(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Working)
{
    ui->setupUi(this);
}

Working::~Working()
{
    delete ui;
}
