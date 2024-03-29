#include "dbmanager.h"

#define DB_TYPE   "QSQLITE"
#define TAB_NAME  "Records"  // Table name
#define TAB_ID    "Id"       // 1 col - Table id
#define TAB_MATCH "Match"    // 2 col - Match to previous image
#define TAB_HASH  "Hash"     // 3 col - Image hash
#define TAB_IMAGE "Image"    // 4 col - Image
#define TAB_DATE  "DateTime" // 5 col - Date and time


DbManager::DbManager( const QString & path )
    : m_dbPath( path )
{
    {
        bool success = false;
        if( !QFile( m_dbPath ).exists() )
        {
            success = createDb();
        }
        else
        {
            success = openDb();
        }

        qDebug() << ( success ? "Database: connection ok" : "Error: connection with database failed" );
    }
}

DbManager::~DbManager()
{
    m_mainDb.close();
}

bool DbManager::openDb()
{
    bool isConnected = false;
    QString conn = QString( QSqlDatabase::defaultConnection );
    if( QSqlDatabase::contains( conn ) )
    {
        m_mainDb = QSqlDatabase::database( conn );
        isConnected = true;
    }
    else
    {
        m_mainDb = QSqlDatabase::addDatabase( DB_TYPE, conn );
        m_mainDb.setDatabaseName( m_dbPath );
        isConnected = m_mainDb.open();
    }
    return isConnected;
}

bool DbManager::createDb()
{
    return openDb() && createTable();
}

bool DbManager::createTable()
{
    QSqlQuery query;
    if( !query.exec( "CREATE TABLE " TAB_NAME " ("
                          TAB_ID    " INTEGER PRIMARY KEY AUTOINCREMENT, "
                          TAB_MATCH " REAL NOT NULL, "
                          TAB_HASH  " TEXT NOT NULL, "
                          TAB_IMAGE " BLOB NOT NULL, "
                          TAB_DATE  " TEXT NOT NULL )" ) )
    {
        qDebug() << query.lastError().text();
        return false;
    }
    else
    {
        return true;
    }
}

void DbManager::inserIntoTable( const QVariantList & data )
{
    QSqlQuery query;

    query.prepare( "INSERT INTO records ( " TAB_MATCH "," TAB_HASH "," TAB_IMAGE "," TAB_DATE " ) "
                   "VALUES ( :" TAB_MATCH ", :" TAB_HASH ", :" TAB_IMAGE ", :" TAB_DATE " )" );

    query.bindValue( ":" TAB_MATCH, data[ 0 ].toFloat() );
    query.bindValue( ":" TAB_HASH,  data[ 1 ].toString() );
    query.bindValue( ":" TAB_IMAGE, data[ 2 ].toByteArray() );
    query.bindValue( ":" TAB_DATE,  data[ 3 ].toString() );

    if( !query.exec() )
    {
        qDebug() << "error inserting table into " << TAB_NAME;
        qDebug() << query.lastError().text();
    }
}

QByteArray DbManager::getImageLatestRecord()
{
    QSqlQuery query;
    QString queryString = "SELECT " TAB_IMAGE " FROM " TAB_NAME " WHERE " TAB_ID ""
                              " = (SELECT MAX( " TAB_ID " ) FROM " TAB_NAME " )";

    if( !query.exec( queryString ) )
    {
        qDebug() << "Error getting latest image from " << TAB_NAME;
        qDebug() << query.lastError().text();
    }

    QByteArray out;
    while( query.next() )
    {
        QSqlRecord record = query.record();
        out = record.value( 0 ).toByteArray();
    }

    return out;
}

int DbManager::getSize()
{
    QSqlQuery query;
    QString queryString = "SELECT COUNT(*) FROM " TAB_NAME;
    query.exec("SELECT COUNT(*) FROM SOME_TABLE");

    if( !query.exec( queryString ) )
    {
        qDebug() << "Error getting rows count from " << TAB_NAME;
        qDebug() << query.lastError().text();
    }

    int rowCount = 0;
    if( query.first() )
    {
        rowCount = query.value( 0 ).toInt();
    }

    return rowCount;
}

float DbManager::getMatch( int row )
{
    QSqlQuery query;
    QString queryString = QString( "SELECT " TAB_MATCH " FROM " TAB_NAME " WHERE " TAB_ID ""
                              " = (%1)" ).arg( row );

    if( !query.exec( queryString ) )
    {
        qDebug() << "Error getting match with id " << row << "from " << TAB_NAME;
        qDebug() << query.lastError().text();
    }

    float out = 0;

    if( query.first() )
    {
        QSqlRecord record = query.record();
        out = record.value( 0 ).toFloat();
    }

    return out;
}

QString DbManager::getHash( int row )
{
    QSqlQuery query;
    QString queryString = QString( "SELECT " TAB_HASH " FROM " TAB_NAME " WHERE " TAB_ID ""
                              " = (%1)" ).arg( row );

    if( !query.exec( queryString ) )
    {
        qDebug() << "Error getting hash with id " << row << "from " << TAB_NAME;
        qDebug() << query.lastError().text();
    }

    QString out = "";

    if( query.first() )
    {
        QSqlRecord record = query.record();
        out = record.value( 0 ).toString();
    }

    return out;
}

QByteArray DbManager::getImageData( int row )
{
    QSqlQuery query;
    QString queryString = QString( "SELECT " TAB_IMAGE " FROM " TAB_NAME " WHERE " TAB_ID ""
                              " = (%1)" ).arg( row );

    if( !query.exec( queryString ) )
    {
        qDebug() << "Error getting image data with id " << row << "from " << TAB_NAME;
        qDebug() << query.lastError().text();
    }

    QByteArray out = "";

    if( query.first() )
    {
        QSqlRecord record = query.record();
        out = record.value( 0 ).toByteArray();
    }

    return out;
}

QString DbManager::getDateTime( int row )
{
    QSqlQuery query;
    QString queryString = QString( "SELECT " TAB_DATE " FROM " TAB_NAME " WHERE " TAB_ID ""
                              " = (%1)" ).arg( row );

    if( !query.exec( queryString ) )
    {
        qDebug() << "Error getting date and time with id " << row << "from " << TAB_NAME;
        qDebug() << query.lastError().text();
    }

    QString out = "";

    if( query.first() )
    {
        QSqlRecord record = query.record();
        out = record.value( 0 ).toString();
    }

    return out;
}

