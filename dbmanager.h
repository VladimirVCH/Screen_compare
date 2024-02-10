#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>

class DbManager
{

    public:
        explicit DbManager( const QString & path );
        ~DbManager();

        void inserIntoTable( const QVariantList & data );
        QByteArray getImageLatestRecord();
        int getSize();
        float getMatch( int row );
        QString getHash( int row );
        QByteArray getImageData( int row );
        QString getDateTime( int row );
        void getAllData(); //test method

    private:
        QSqlDatabase m_mainDb;
        bool openDb();
        bool createDb();
        bool createTable();

        QString m_dbPath;

};

#endif // DBMANAGER_H
