#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QString>

#include "Defines/Util.h"
#include "Defines/define_picture.h"
#include "Util/UtilString.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // Get application working directory
    QString applicationDirPath = application.applicationDirPath();

    // Application Icon
    QString applicationIconPath = applicationDirPath + "/" + PICTURE_APPLICATION_ICON;
    const QIcon *icon = new QIcon( applicationIconPath );
    application.setWindowIcon( *icon );

    // For Mac Operating System
    if( UtilString::isStringFinishBy( applicationDirPath, STRING_MACOSX ) )
        applicationDirPath = UtilString::removeToBeginning( applicationDirPath, '/', 3 );

    MainWindow w( applicationDirPath );
    w.show();
    
    return application.exec();
}
