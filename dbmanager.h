#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>

class DbManager : public QObject
{
    Q_OBJECT

    public:

        DbManager( const QString & path, QObject * parent = nullptr );
        ~DbManager();

        void inserIntoTable( const QVariantList & data );
        QByteArray getImageLatestRecord();
        void getAllData();

    private:

        QSqlDatabase m_mainDb;
        bool openDb();
        bool createDb();
        bool createTable();

        QString m_dbPath;


    signals:

};

#endif // DBMANAGER_H
