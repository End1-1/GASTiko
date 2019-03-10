#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QVariant>
#include <QSqlQuery>
#include <QDate>

class Database
{
public:
    Database();

    ~Database();

    QMap<QString, QVariant> fBind;

    bool prepare(const QString &query);

    QVariant &operator[](const QString &name);

    bool exec();

    bool next();

    int getInteger(int column);

    double getDouble(int column);

    QDate getDate(int column);

    QString getString(int column);

    QString fLastError;

private:
    QSqlDatabase fDatabase;

    static int fCounter;

    QString fConnectionName;

    QSqlQuery fQuery;
};

#endif // DATABASE_H
