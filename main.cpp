#include "appwrapper.h"

int main( int argc, char * argv[] )
{
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );

    QGuiApplication app( argc, argv );

    AppWrapper wrapper( app );

    if ( !wrapper.initialize() )
    {
        return -1;
    }

    return app.exec();
}
