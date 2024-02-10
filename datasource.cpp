#include <QDebug>
#include "datasource.h"
#include <QImage>
#include <QPixmap>
#include <QScreen>

DataSource::DataSource( const QGuiApplication & app, QObject * parent )
    : QObject( parent ), m_app( app )
{
    m_dbManager.reset( new DbManager( QDir::tempPath() + "test.db" ) );

    m_newPixmap.loadFromData( m_dbManager->getImageLatestRecord() );

    m_processingThread.reset( new QThread() );

    m_imageProcessing.reset( new ImageProcessing() );

    m_imageProcessing->moveToThread( m_processingThread.get() );

    // update DB after image processing
    connect( m_imageProcessing.get(), &ImageProcessing::processingReady, this, &DataSource::updateDb );
    // start image processing after screenshot
    connect( this, &DataSource::screenShotIsReady, m_imageProcessing.get(), &ImageProcessing::startProcessing );
    // finish thread
    connect( this, &DataSource::finished, m_processingThread.get(), [ = ]{
        m_processingThread->quit();
        m_processingThread->wait(); } );

    m_processingThread->start();
}

DataSource::~DataSource()
{
    emit finished();
}

void DataSource::startTimer()
{
    m_timer.reset( new QTimer() );
    connect( m_timer.get(), & QTimer::timeout, this, & DataSource::getImageOnTimer );
    m_timer->start( m_timerFreq ); //start timer - in msec
    qDebug() << "Timer started";
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
    emit screenShotIsReady( m_prevPixmap, m_newPixmap );

    qDebug() << "Screenshot captured";
}

void DataSource::updateDb( QVariantList list )
{
    emit preItemAdded();
    m_dbManager->inserIntoTable( list );
    emit postItemAdded();

    qDebug() << "Data Base updated";
}

void DataSource::takeScreenShot()
{
    m_prevPixmap = m_newPixmap;
    QScreen * screen = m_app.primaryScreen();
    m_newPixmap = screen->grabWindow( 0 );

}
