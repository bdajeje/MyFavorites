#include "enginecron.h"

EngineCron::EngineCron( EngineOnline *engineOnline, EngineProperties *engineProperties, EngineTvshow *engineTvShow )
{
    this->engineOnline = engineOnline;
    this->engineProperties = engineProperties;
    this->engineTvShow = engineTvShow;
    this->timer = new QBasicTimer();
    this->startCron();
}


EngineCron::~EngineCron()
{
    delete this->timer;
}


void EngineCron::startCron()
{
    int interval = this->engineProperties->getUser( "CRON_UPDATEINTERVAL" ).toInt();
    if( interval != 0 )
    {
        QDateTime currentDate = QDateTime::currentDateTime();
        QDateTime lastDate = QDateTime::fromString( this->engineProperties->getUser( "CRON_LASTCHECK" ), DATE_FORMAT );
        int time = currentDate.toTime_t() - lastDate.toTime_t();

        if( time > interval )
            time = 0;
        else if( time > 0 )
            time = interval - time;
        else time = interval;

        this->timer->start( time * 1000, this );
    }
}


void EngineCron::timerEvent( QTimerEvent *event )
{    
    event->accept();
    this->checkOnline();
}


void EngineCron::clickUpdate()
{
    this->checkOnline();
}


void EngineCron::checkOnline()
{
    cout << "Checking online updates" << endl;
    this->timer->stop();

    this->engineProperties->setData( "CRON_LASTCHECK", QVariant( QDateTime::currentDateTime().toString( DATE_FORMAT ) ) );
    int j, i, size = this->engineTvShow->countCategories();
    for( i = 0; i < size; i++ )
    {
        int showSize = this->engineTvShow->count( this->engineTvShow->getCategory(i) );
        for( j = 0; j < showSize; j++ )
        {
            TVShow *tvshow = this->engineTvShow->get( j, this->engineTvShow->getCategory(i) );
            if( tvshow->getCheck() && tvshow->getId() != 0 )
                this->engineOnline->updateShow( tvshow->getId(), tvshow->getName() );
        }
    }

    if( !this->timer->isActive() )
        this->timer->start( this->engineProperties->getUser( "CRON_UPDATEINTERVAL" ).toInt() * 1000, this );
}
