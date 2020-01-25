#include "database.h"
#include <QMessageBox>
#include <QSqlError>
#include <QApplication>
#include <QObject>
#include <QDebug>

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
    if (fDatabase.isOpen()) {
        fDatabase.commit();
    }
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
#ifdef QT_DEBUG
    qDebug() << lastQuery(&fQuery);
#endif
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

const QString Database::lastQuery(QSqlQuery *q) const
{
    QString sql = q->lastQuery();
    QMapIterator<QString, QVariant> it(q->boundValues());
    while (it.hasNext()) {
        it.next();
        QVariant value = it.value();
        switch (it.value().type()) {
        case QVariant::String:
            value = QString("'%1'").arg(value.toString().replace("'", "''"));
            break;
        case QVariant::Date:
            value = QString("'%1'").arg(value.toDate().toString("yyyy-MM-dd"));
            break;
        case QVariant::DateTime:
            value = QString("'%1'").arg(value.toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            break;
        case QVariant::Double:
            value = QString("%1").arg(value.toDouble());
            break;
        case QVariant::Int:
            value = QString("%1").arg(value.toInt());
            break;
        case QVariant::Time:
            value = QString("'%1'").arg(value.toTime().toString("HH:mm:ss"));
            break;
        default:
            break;
        }
        sql.replace(QRegExp(it.key() + "\\b"), value.toString());
    }
    return sql;
}
