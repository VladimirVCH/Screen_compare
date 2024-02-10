#include <QDebug>
#include "datasource.h"
#include <QImage>
#include <QPixmap>
#include <QScreen>

DataSource::DataSource( const QGuiApplication & app, QObject * parent )
    : QObject( parent ), m_app( app )
{
    m_dbManager.reset( new DbManager( "/tmp/test.db" ) ); //TODO adjust path

    m_newPixmap.loadFromData( m_dbManager->getImageLatestRecord() );
}

void DataSource::startTimer()
{
    m_timer.reset( new QTimer() );
    connect( m_timer.get(), & QTimer::timeout, this, & DataSource::getImageOnTimer );
    m_timer->start( m_timerFreq ); //start timer - in msec
    qDebug() << "Timer started";
//    m_dbManager->getAllData();
}

void DataSource::stopTimer()
{
    if( m_timer )
    {
        m_timer->stop();
        qDebug() << "Timer stopped";
    }
}

int DataSource::getSize()
{
    return m_dbManager->getSize();
}

float DataSource::getMatch( int row )
{
    return m_dbManager->getMatch( row + 1 ); // id numeration in DB starts from 1
}

QString DataSource::getHash( int row )
{
    return m_dbManager->getHash( row + 1 ); // id numeration in DB starts from 1
}

bool DataSource::getImageData( int row )
{
    auto imageData = m_dbManager->getImageData( row + 1 ); // id numeration in DB starts from 1
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData( imageData );
    if( m_imageProvider )
    {
        m_imageProvider->updateImage( outPixmap );
        return true;
    }
    return false;
}

QString DataSource::getDateTime( int row )
{
    return m_dbManager->getDateTime( row + 1 ); // id numeration in DB starts from 1
}

void DataSource::setImageProvider( QmlImageProvider * imageProvider )
{
    m_imageProvider = imageProvider;
}

void DataSource::getImageOnTimer()
{
    takeScreenShot();

    QVariantList list;
    list.append( compare() ); // Match
    auto byteArr = latestPixmapToByteArray();
    list.append( getHash( byteArr ) ); // Hash
    list.append( byteArr ); // Image data
    list.append( getDateTime() ); // DateTime

    emit preItemAdded();
    m_dbManager->inserIntoTable( list );
    emit postItemAdded();

    qDebug() << "Screenshot captured";
}

void DataSource::takeScreenShot()
{
    m_prevPixmap = m_newPixmap;
    QScreen * screen = m_app.primaryScreen();
    m_newPixmap = screen->grabWindow( 0 );

}

float DataSource::compare()
{
    auto latestImage = m_newPixmap.toImage();
    auto prevImage = m_prevPixmap.toImage();

    if( ( latestImage.width() != prevImage.width() ) || ( latestImage.height() != prevImage.height() ) )
    {
        return 0;
    }

    quint32 matchPix = 0;
    quint32 totalPix = latestImage.width() * latestImage.height();
    for( auto i = 0; i < latestImage.width() ; ++i )
    {
        for( auto j = 0; j < latestImage.height(); ++j )
        {
            if( latestImage.pixel( i, j ) == prevImage.pixel( i, j ) )
            {
                ++matchPix;
            }
        }
    }

    return float( 100.00 ) * matchPix / totalPix;
}

QByteArray DataSource::latestPixmapToByteArray()
{
    QByteArray inByteArray;
    QBuffer inBuffer( & inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    m_newPixmap.save( & inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format
    return inByteArray;
}

QString DataSource::getDateTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return dateTime.toString( Qt::ISODate );
}

QByteArray DataSource::getHash( const QByteArray & imgData )
{
    return QCryptographicHash::hash( imgData, QCryptographicHash::Md5 ).toHex();
}
