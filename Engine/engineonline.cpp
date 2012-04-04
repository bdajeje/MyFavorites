#include "engineonline.h"

EngineOnline::EngineOnline( const QString &applicationDirPath, EngineInformation *engineInformation )
{
    this->applicationDirPath = applicationDirPath;
    this->engineInformation = engineInformation;
    this->networkManager = new QNetworkAccessManager( this );
    this->connect( this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT( requestFinish(QNetworkReply*) ) );
}


EngineOnline::~EngineOnline()
{
    delete this->networkManager;
}


void EngineOnline::updateShow( int id, const QString &name )
{
    //cout << "add request update: " << id << endl;
    this->queue.enqueue( Request( TYPE_GET_EPISODES, id, name ) );
    this->queue.enqueue( Request( TYPE_GET_DESCRIPTION, id, name ) );
    if( this->queue.size() == 2 )
        this->proceedNextRequest();
}


void EngineOnline::requestFinish( QNetworkReply *reply )
{    
    cout << "request finish: "<< endl;

    if( !this->queue.empty() )
    {
        if( this->queue.head().getType() == TYPE_GET_EPISODES )
            this->saveShowFile( reply, PATH_SERIES_EPISODE );
        else if( this->queue.head().getType() == TYPE_GET_DESCRIPTION )
            this->saveShowFile( reply, PATH_SERIES_DESCRIPTION );

        this->queue.dequeue();
        this->engineInformation->endInformation();        
    }
    else cout << "empty queue ?" << endl;

    this->proceedNextRequest();
}


void EngineOnline::proceedNextRequest()
{    
    if( !this->queue.empty() )
    {
        if( this->queue.head().getType() == TYPE_GET_EPISODES )
        {
            this->networkManager->get( QNetworkRequest( QUrl( URL_EPISODELIST + this->queue.head().getValue().toString() ) ) );
            this->engineInformation->newInformation( this->queue.head().getType(), this->queue.head().getName() );
        }
        else if( this->queue.head().getType() == TYPE_GET_DESCRIPTION )
        {
            this->networkManager->get( QNetworkRequest( QUrl( URL_SHOWDESCRIPTION + this->queue.head().getValue().toString() ) ) );
            this->engineInformation->newInformation( this->queue.head().getType(), this->queue.head().getName() );
        }
    }
}


void EngineOnline::saveShowFile( QNetworkReply *reply, const string &pathToFile )
{
    QFile file( ( this->applicationDirPath.toStdString() + pathToFile + this->queue.head().getValue().toString().toStdString() ).c_str() );
    if( file.open( QIODevice::WriteOnly|QIODevice::Truncate ) )
    {
        QByteArray byteArray = reply->readAll();        
        file.write( byteArray );
        file.close();
        emit showUpdated( this->queue.head().getValue().toInt() );        
    }
    else cout << "ERROR: can't truncate file: " << file.fileName().toStdString() << endl;
}
