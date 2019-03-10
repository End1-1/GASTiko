#ifndef WORKING_H
#define WORKING_H

#include <QDialog>

namespace Ui {
class Working;
}

class Working : public QDialog
{
    Q_OBJECT

public:
    explicit Working(QWidget *parent = 0);
    ~Working();

private:
    Ui::Working *ui;
};

#endif // WORKING_H
