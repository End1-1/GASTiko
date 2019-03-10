#include "config.h"
#include <QSettings>
#include <QWidget>

QSettings fSettings("GASTiko", "GASTiko");
QWidget *Config::fParentWidget = 0;

Config::Config()
{

}

bool Config::go()
{
    return fSettings.value("var").toString() == "macunabrdosh";
}

QString Config::slogan()
{
    return fSettings.value("slogan").toString();
}

void Config::setSlogan(const QString &value)
{
    fSettings.setValue("slogan", value);
}

double Config::scaleFactor()
{
    return fSettings.value("scale").toDouble();
}

void Config::setScaleFactor(double value)
{
    fSettings.setValue("scale", value);
}

QWidget *Config::parentWidget()
{
    return fParentWidget;
}

void Config::setParentWidget(QWidget *parent)
{
    fParentWidget = parent;
}
