#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class QWidget;

class Config
{
public:
    Config();

    static bool go();

    static QString slogan();

    static void setSlogan(const QString &value);

    static double scaleFactor();

    static void setScaleFactor(double value);

    static QWidget *parentWidget();

    static void setParentWidget(QWidget *parent);

private:
    static QWidget *fParentWidget;
};

#endif // CONFIG_H
