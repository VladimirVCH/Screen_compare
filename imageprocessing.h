#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>
#include <QPixmap>
#include <QVariantList>

/**
 * @brief The ImageProcessing class used to process images in separate thread
 */
class ImageProcessing : public QObject
{
    Q_OBJECT

    public:
        explicit ImageProcessing( QObject * parent = nullptr );
        ~ImageProcessing() = default;

    signals:
        void processingReady( QVariantList list );

    public slots:
        void startProcessing( const QPixmap & prevPixmap, const QPixmap & newPixmap );

    private:
        float compare( const QPixmap & prevPixmap, const QPixmap & newPixmap );
        QByteArray latestPixmapToByteArray( const QPixmap & newPixmap );
        QString getDateTime();
        QByteArray getHash( const QByteArray & imgData );
};

#endif // IMAGEPROCESSING_H
