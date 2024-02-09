#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QHash>
#include <QCryptographicHash>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QScreen>

#include "dbmanager.h"

QByteArray getHash( const QByteArray & imgData )
{
    return QCryptographicHash::hash( imgData, QCryptographicHash::Md5 ).toHex();
}

uint takeScreenshot()
{
    QScreen * screen = QGuiApplication::primaryScreen();
    QPixmap pixmap;
//    screen = QGuiApplication::primaryScreen();
    auto geom = screen->geometry();
    pixmap = screen->grabWindow( 0, geom.x(), geom.y(), geom.width(), geom.height() );

    static uint count = 3;
    ++count;
    // use local project folder
    pixmap.save( QString( "/tmp/test%1.png" ).arg( count ), "png" );

    return count;
}

QByteArray anotherScreenShot( const QGuiApplication & app )
{
    QScreen * screen = app.primaryScreen();
    QPixmap inPixmap = screen->grabWindow( 0 );
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format
    return inByteArray;
}

QByteArray getFileImageData( const QString & path )
{
    QPixmap inPixmap;
    inPixmap.load( path, "PNG" );
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format
    return inByteArray;
}

QPixmap getImageFromData( const QByteArray & imageData )
{
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData( imageData );
    return outPixmap;
}

float compare( QPixmap latest, QPixmap prev )
{
    auto latestImage = latest.toImage();
    auto prevImage = prev.toImage();

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

int main(int argc, char *argv[])
{
//#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif
//    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);

//    return app.exec();
    QGuiApplication app(argc, argv); //remove later !!!

    DbManager dbManager( "/tmp/test.db" );

    QVariantList list;

    auto latest = anotherScreenShot( app ); // need to get QPixmap directly here
    auto prev = dbManager.getImageLatestRecord();

    list.append( compare( getImageFromData( latest ), getImageFromData( prev ) ) ); //match
    list.append( getHash( latest ) ); // hash
    list.append( latest ); // image data

    QDateTime dateTime = QDateTime::currentDateTime();
    auto strTime = dateTime.toString( Qt::ISODate );
    list.append( strTime ); // date time

    dbManager.inserIntoTable( list );

    dbManager.getAllData();

    return 0;
}
