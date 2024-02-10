#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QTimer>
#include <QGuiApplication>

#include "dbmanager.h"
#include "qmlimageprovider.h"
#include "imageprocessing.h"

/**
 * @brief The DataSource class is making all image actions and operated with the DataBase
 */
class DataSource : public QObject
{
    Q_OBJECT

    public:
        explicit DataSource( const QGuiApplication & app, QObject * parent = nullptr );
        ~DataSource();
        Q_INVOKABLE void startTimer();
        Q_INVOKABLE void stopTimer();
        int getSize();
        float getMatch( int row );
        QString getHash( int row );
        bool getImageData( int row );
        QString getDateTime( int row );
        void setImageProvider( QmlImageProvider * imageProvider );

    signals:
        void preItemAdded();
        void postItemAdded();
        void screenShotIsReady( const QPixmap & prevPixmap, const QPixmap & newPixmap );
        void finished();

    public slots:
        void updateDb( QVariantList list );

    private slots:
        void getImageOnTimer();

    private :
        void takeScreenShot();

        QScopedPointer< DbManager > m_dbManager;
        QScopedPointer< QTimer > m_timer;
        QScopedPointer< QThread > m_processingThread;
        QScopedPointer< ImageProcessing > m_imageProcessing;
        QmlImageProvider * m_imageProvider;
        uint32_t m_timerFreq = 60 * 1000; //60 sec in ms
        const QGuiApplication & m_app;
        QPixmap m_prevPixmap;
        QPixmap m_newPixmap;
};

#endif // DATASOURCE_H
