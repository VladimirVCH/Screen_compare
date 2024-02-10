#include "datamodel.h"

DataModel::DataModel( QObject * parent ) : QAbstractListModel( parent )
{
}

int DataModel::rowCount( const QModelIndex & parent ) const
{
    Q_UNUSED( parent );
    return m_datasource->getSize();
}

QVariant DataModel::data( const QModelIndex &index, int role ) const
{
    if( index.row() < 0 || index.row() >= m_datasource->getSize() )
    {
        return QVariant();
    }
    else if( role == matchDataRole )
    {
        return m_datasource->getMatch( index.row() );
    }
    else if( role == hashDataRole )
    {
        return m_datasource->getHash( index.row() );
    }
    else if( role == dateTimeDataRole )
    {
        return m_datasource->getDateTime( index.row() );
    }
    else if( role == imageDataRole )
    {
        return m_datasource->getImageData( index.row() );
    }
    else
    {
        return QVariant();
    }
}

Qt::ItemFlags DataModel::flags( const QModelIndex & index ) const
{
    if( !index.isValid() )
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEditable;
}

QHash< int, QByteArray > DataModel::roleNames() const
{
    QHash< int, QByteArray > roles;
    roles[ matchDataRole ] =    "matchData";
    roles[ hashDataRole ] =     "hashData";
    roles[ imageDataRole ] =    "imageData";
    roles[ dateTimeDataRole ] = "timeData";
    return roles;
}

DataSource * DataModel::datasource() const
{
    return m_datasource;
}

void DataModel::setDatasource( DataSource * datasource )
{
    beginResetModel();

    if( m_datasource && signalsConnected )
    {
        m_datasource->disconnect( this );
    }

    m_datasource = datasource;

    connect( m_datasource, & DataSource::preItemAdded, this,
             [ = ](){ const int index = m_datasource->getSize();
                      beginInsertRows( QModelIndex(), index, index ); } );

    connect( m_datasource, & DataSource::postItemAdded, this,
             [ = ](){ endInsertRows(); } );

    endResetModel();
}
