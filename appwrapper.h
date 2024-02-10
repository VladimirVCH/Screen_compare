#ifndef APPWRAPPER_H
#define APPWRAPPER_H
#include <QQmlApplicationEngine>
#include <QObject>
#include "datamodel.h"

class AppWrapper : public QObject
{
    Q_OBJECT

    public:
        explicit AppWrapper( const QGuiApplication & app, QObject * parent = nullptr );
        bool initialize();

    private:
        QQmlApplicationEngine m_engine;
        DataModel m_dataModel;
        const QGuiApplication & m_app;
};

#endif // APPWRAPPER_H
