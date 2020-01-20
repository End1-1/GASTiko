#include "database.h"
#include <QMessageBox>
#include <QSqlError>
#include <QApplication>
#include <QObject>

int Database::fCounter = 0;

Database::Database()
{
    fConnectionName = "db" + QString::number(fCounter++);
    fDatabase = QSqlDatabase::addDatabase("QIBASE", fConnectionName);
#ifdef QT_DEBUG
    fDatabase.setHostName("127.0.0.1");
    fDatabase.setDatabaseName("C:\\Projects\\GASTiko\\db.fdb");
#else
    fDatabase.setHostName("127.0.0.1");
    fDatabase.setDatabaseName(qApp->applicationDirPath() + "\\db.fdb");
#endif
    fDatabase.setUserName("SYSDBA");
    fDatabase.setPassword("masterkey");
    fDatabase.setConnectOptions("lc_ctype=utf8");
    if (!fDatabase.open()) {
        QMessageBox::critical(0, QObject::tr("Database error"), fDatabase.lastError().databaseText());
        return;
    }
    fQuery = QSqlQuery(fDatabase);
}

Database::~Database()
{
    fDatabase = QSqlDatabase::addDatabase("QIBASE");
    QSqlDatabase::removeDatabase(fConnectionName);
}

bool Database::prepare(const QString &query)
{
    bool result = fQuery.prepare(query);
    if (!result) {
        fLastError = fQuery.lastError().databaseText();
    }
    return result;
}

QVariant &Database::operator[](const QString &name)
{
    return fBind[name];
}

bool Database::exec()
{
    for (QMap<QString, QVariant>::const_iterator it = fBind.begin(); it != fBind.end(); it++) {
        fQuery.bindValue(it.key(), it.value());
    }
    bool result = fQuery.exec();
    if (!result) {
        fLastError = fQuery.lastError().databaseText();
    }
    return result;
}

bool Database::next()
{
    return fQuery.next();
}

int Database::getInteger(int column)
{
    return fQuery.value(column).toInt();
}

double Database::getDouble(int column)
{
    return fQuery.value(column).toDouble();
}

QDate Database::getDate(int column)
{
    return fQuery.value(column).toDate();
}

QString Database::getString(int column)
{
    return fQuery.value(column).toString();
}
