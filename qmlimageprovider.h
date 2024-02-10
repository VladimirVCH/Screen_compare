#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H

#include <QQuickImageProvider>

/**
 * @brief The QmlImageProvider class is used to shop images in QML
 */
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
