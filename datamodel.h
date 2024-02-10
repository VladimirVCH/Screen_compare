#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "datasource.h"

/**
 * @brief The DataModel class is used to ensure dynamic update of model for QML GUI
 */
class DataModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( DataSource * datasource READ datasource WRITE setDatasource )

    enum ModelRoles
    {
        matchDataRole = Qt::UserRole + 1,
        hashDataRole,
        imageDataRole,
        dateTimeDataRole
    };

    public:
        explicit DataModel( QObject * parent = nullptr );
        int rowCount( const QModelIndex & parent = QModelIndex() ) const;
        QVariant data( const QModelIndex & index, int role ) const;
        Qt::ItemFlags flags( const QModelIndex & index ) const;
        QHash<int, QByteArray> roleNames() const;
        DataSource * datasource() const;
        void setDatasource( DataSource * datasource );

    private :
        DataSource * m_datasource;
        bool signalsConnected { false };
};

#endif // DATAMODEL_H
