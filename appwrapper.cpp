#include <QNetworkRequest>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "appwrapper.h"

AppWrapper::AppWrapper( const QGuiApplication & app, QObject * parent )
    : QObject( parent ), m_app( app )
{
}

bool AppWrapper::initialize()
{
    DataSource * ds = new DataSource( m_app, this );
    m_dataModel.setDatasource( ds );
    m_engine.rootContext()->setContextProperty( "scrShotModel", &m_dataModel );
    m_engine.rootContext()->setContextProperty( "scrShotDataSource", ds );

    QmlImageProvider * imageProvider = new QmlImageProvider();
    ds->setImageProvider( imageProvider );
    m_engine.addImageProvider( QLatin1String( "imageProvider" ), imageProvider );

    m_engine.load( QUrl( QStringLiteral( "qrc:/main.qml" ) ) );
    if( m_engine.rootObjects().isEmpty() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

