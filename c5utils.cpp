#include "c5utils.h"
#include <QFile>
#include <QDateTime>

C5Utils __c5utils;

C5Utils::C5Utils()
{

}

void C5Utils::writeErrorLog(const QString &text)
{
    QFile f("log.log");
    if (f.open(QIODevice::Append)) {
        f.write(QDateTime::currentDateTime().toString(FORMAT_DATETIME_TO_STR).toUtf8());
        f.write(" ");
        f.write(text.toUtf8());
        f.write("\r\n");
        f.close();
    }
}

QString password(const QString &value)
{
    QByteArray ba(value.toUtf8());
    return QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex();
}

QString hostusername()
{
    QString username = getenv("USER");
    if (username.isEmpty()) {
        username = getenv("USERNAME");
    }
    return username;
}
