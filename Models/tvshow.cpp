#include "tvshow.h"

TVShow::TVShow()
{
    id = 0;
    name = "Unknown";
    category = "Unknown";
    episode = 0;
    season = 0;
    checkable = false;
    stars = 0;
    this->news = false;
}


TVShow::TVShow( int id, const QString &name, const QString &category, bool checkable, int stars )
{
    this->id = id;
    this->name = name;
    this->category = category;
    this->episode = 0;
    this->season = 1;
    this->checkable = checkable;
    this->stars = stars;
    this->news = false;
}


TVShow::~TVShow()
{
    int i, size = this->seasons.size();
    for( i = 0; i < size; i++ )
    {
        int j, sizeJ = this->seasons[i].size();
        for( j = 0; j < sizeJ; j++ )
            delete this->seasons[i][j];
    }
}


void TVShow::addEpisode( const QString &name, unsigned int number, unsigned int season, const QDate &date )
{ 
    if( this->seasons.size() < season )
        this->seasons.resize( season );
    if( number >= this->seasons.at( season - 1 ).size() )
        this->seasons.at( season - 1 ).push_back( new Episode( name, number, season, date ) );
}


int TVShow::isAfterLast( unsigned int season, unsigned int episode ) const // 0 inferior, 1 egal, 2 after
{
    const Episode *lastEpisode = this->getLastInDate();
    return this->compare( lastEpisode, season, episode );
}


int TVShow::compare( const Episode *episode, unsigned int season, unsigned int episodeNumber ) const
{
    if( episode->getSeason() == season )
    {
        if( episode->getNumber() > episodeNumber )
            return 0;
        else if( episode->getNumber() == episodeNumber )
            return 1;
        else return 2;
    }
    else if( episode->getSeason() < season )
        return 2;
    else return 0;
}


int TVShow::isAfterCurrent( unsigned int season, unsigned int episode ) // 0 inferior, 1 egal, 2 after
{
    const Episode *tmp = this->getEpisode( this->season, this->episode );
    if( tmp != NULL )
        return this->compare( tmp, season, episode );
    else return -1;
}


Episode* TVShow::getEpisode( unsigned int season, unsigned int episode )
{
    if( episode <= 0 )
    {
        season -= season;
        if( season > 0 )
            episode = this->seasons.at(season-1).size();
        else return NULL;
    }

    if( season-1 < this->seasons.size() && episode-1 < this->seasons.at(season-1).size() )
        return this->seasons.at(season-1).at(episode-1);
    else return NULL;
}


const Episode* TVShow::getLastInDate() const
{
    QDate currentDate = QDate::currentDate();
    Episode *lastEpisode = NULL;
    int j, sizeJ = this->seasons.size();
    for( j = 0; j < sizeJ; j++ )
    {
        int i, size = this->seasons.at(j).size();
        for( i = 0; i < size; i++ )
        {
            if( this->seasons.at(j).at(i)->getDate() > currentDate )
                break;
            lastEpisode = this->seasons.at(j).at(i);
        }
    }
    return lastEpisode;
}


bool TVShow::isUpToDate()
{            
    const Episode *lastEpisode = this->getLastInDate();

    if( lastEpisode != NULL )
        return ( this->isAfterCurrent( lastEpisode->getSeason(), lastEpisode->getNumber() ) == 2 ) ? false : true;
    else return true;
}
