#include "maindialog.h"
#include "config.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    if (!Config::go()) {
        return -1;
    }
    QApplication a(argc, argv);

    QTranslator t;
    t.load(":/GASTiko.qm");
    a.installTranslator(&t);

    MainDialog w;
    w.showMaximized();

    return a.exec();
}
