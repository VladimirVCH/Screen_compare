#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H
#include <QQuickImageProvider>


class QmlImageProvider : public QQuickImageProvider
{
    public:
        QmlImageProvider();
        QPixmap requestPixmap( const QString & id, QSize * size, const QSize & requestedSize ) override;
        void updateImage( QPixmap newImage );

    private:
        QPixmap m_Pixmap;
        QPixmap m_NoImage;
};

#endif // QMLIMAGEPROVIDER_H
