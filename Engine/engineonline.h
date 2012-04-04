#ifndef ENGINEONLINE_H
#define ENGINEONLINE_H

#include <QWidget>
#include <QUrl>
#include <QQueue>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>

#include <iostream>

#include "Defines/online.h"
#include "Defines/define.h"
#include "Defines/enum.h"
#include "Models/request.h"
#include "Engine/engineinformation.h"

using namespace std;

class EngineOnline : public QWidget
{
    Q_OBJECT

    public:
        EngineOnline( const QString &applicationDirPath, EngineInformation *engineInformation  );
        ~EngineOnline();

        void updateShow( int id, const QString &name );

    private:
        void proceedNextRequest();
        void saveShowFile( QNetworkReply *reply, const string &pathToFile );

    private:
        EngineInformation *engineInformation;
        QString applicationDirPath;
        QNetworkAccessManager* networkManager;
        QQueue<Request> queue;

    public slots:
        void requestFinish( QNetworkReply *request );

    signals:
        void showUpdated( int id );
};

#endif // ENGINEONLINE_H
