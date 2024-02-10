#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QTimer>
#include <QGuiApplication>

#include "dbmanager.h"
#include "qmlimageprovider.h"

class DataSource : public QObject
{
    Q_OBJECT

    public:
        explicit DataSource( const QGuiApplication & app, QObject * parent = nullptr );
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
        void preItemRemoved(int index);
        void postItemRemoved();

    private slots:
        void getImageOnTimer();

    private :
        void takeScreenShot();
        float compare();
        QByteArray latestPixmapToByteArray();
        QString getDateTime();
        QByteArray getHash( const QByteArray & imgData );

        QScopedPointer< DbManager > m_dbManager;
        QScopedPointer< QTimer > m_timer;
        QmlImageProvider * m_imageProvider;
        uint32_t m_timerFreq = 10 * 1000; //60 sec in ms
        const QGuiApplication & m_app;
        QPixmap m_prevPixmap;
        QPixmap m_newPixmap;
};

#endif // DATASOURCE_H
