#include "imageprocessing.h"
#include <QImage>
#include <QBuffer>
#include <QDateTime>
#include <QCryptographicHash>

ImageProcessing::ImageProcessing( QObject * parent ) : QObject( parent )
{
}

void ImageProcessing::startProcessing( const QPixmap & prevPixmap, const QPixmap & newPixmap )
{
    QVariantList list;
    list.append( compare( prevPixmap, newPixmap ) ); // Match
    auto byteArr = latestPixmapToByteArray( newPixmap );
    list.append( getHash( byteArr ) ); // Hash
    list.append( byteArr ); // Image data
    list.append( getDateTime() ); // DateTime
    emit processingReady( list );
}

float ImageProcessing::compare( const QPixmap & prevPixmap, const QPixmap & newPixmap )
{
    auto latestImage = newPixmap.toImage();
    auto prevImage = prevPixmap.toImage();

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

QByteArray ImageProcessing::latestPixmapToByteArray( const QPixmap & newPixmap )
{
    QByteArray inByteArray;
    QBuffer inBuffer( & inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    newPixmap.save( & inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format
    return inByteArray;
}

QString ImageProcessing::getDateTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return dateTime.toString( Qt::ISODate );
}

QByteArray ImageProcessing::getHash( const QByteArray & imgData )
{
    return QCryptographicHash::hash( imgData, QCryptographicHash::Md5 ).toHex();
}
