#include "qmlimageprovider.h"

QmlImageProvider::QmlImageProvider() : QQuickImageProvider( QQuickImageProvider::Pixmap )
{
    m_NoImage = QPixmap( 100, 100 );
    m_NoImage.fill( QColor( "black" ) );
}

QPixmap QmlImageProvider::requestPixmap( const QString & id, QSize * size, const QSize & requestedSize)
{
    return m_Pixmap.isNull() ? m_NoImage : m_Pixmap;
}

void QmlImageProvider::updateImage( QPixmap newImage )
{
    m_Pixmap = newImage;
}
