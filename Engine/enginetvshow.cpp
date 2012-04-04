#include "enginetvshow.h"

EngineTvshow::EngineTvshow( const EngineLanguage *engineLanguage, const QString &applicationDirPath )
{
    this->engineLanguage = engineLanguage;
    this->applicationDirPath = applicationDirPath;
    this->initializeSeries();

    /* WAS DONE TO CREATE PROGRESSION FILES
    for( int i = 0; i < this->tvshows.size(); i++ )
    {
        TVShow *show = this->tvshows.at(i);
        QFile file( "/home/jeje/Programmation/C++/MyTVShows-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Release/Objects/Series/Progression/" + QString::number( show->getId() ) );
        const vector< vector<Episode*> >* seasons = show->getSeasons();
        if( file.open( QIODevice::WriteOnly ) )
        {
            for( int j = 0; j < seasons->size(); j++ )
            {
                vector<Episode*> episodes = seasons->at(j);
                for( int k = 0; k < episodes.size(); k++ )
                {
                    bool saw = (show->isAfterCurrent(episodes.at(k)->getSeason(), episodes.at(k)->getNumber()) == 2) ? false : true;
                    QString text = QString::number(episodes.at(k)->getSeason());
                    text.append( "-").append( QString::number(episodes.at(k)->getNumber() ) ).append( "-" ).append(QString::number((saw) ? 1 : 0)).append("\n");
                    file.write( QByteArray( text.toStdString().c_str() ) );
                }
            }
            file.close();
        }else cout << "NON !" << endl;
    }
    */
}


EngineTvshow::~EngineTvshow()
{
    unsigned int i, size = this->tvshows.size();
    for( i = 0; i < size; i++ )
        delete this->tvshows.at(i);
}


void EngineTvshow::initializeSeries()
{
    // Series file
    QFile file( this->applicationDirPath + FILE_SERIES );
    if( file.open( QIODevice::ReadOnly ) )
    {
        char buffer[1024];
        int size = sizeof(buffer), read;
        while( ( read = file.readLine( buffer, size ) ) > 0 )
            this->parseLine( buffer, read );
        file.close();
    }
    else cout << "ERROR: can't open file: " << file.fileName().toStdString() << endl;

    // Serie files episode
    QDir directory( this->applicationDirPath + PATH_SERIES_EPISODE );
    QStringList list = directory.entryList();
    int i, sizeFiles = list.size();
    for( i = 0; i < sizeFiles; i++ )
    {
        if( list.at(i) != "." && list.at(i) != ".." && list.at(i).toInt() != 0 )
            this->readEpisodes( list.at(i).toInt() );
    }

    // Serie files description
    QDir directoryDescription( this->applicationDirPath + PATH_SERIES_DESCRIPTION );
    QStringList listDesciprtion = directoryDescription.entryList();
    sizeFiles = listDesciprtion.size();
    for( i = 0; i < sizeFiles; i++ )
    {
        if( listDesciprtion.at(i) != "." && listDesciprtion.at(i) != ".." && listDesciprtion.at(i).toInt() != 0 )
            this->readDescription( listDesciprtion.at(i).toInt() );
    }

    // Serie files progression
    QDir directoryProgression( this->applicationDirPath + PATH_SERIES_PROGRESSION );
    QStringList listProgression = directoryProgression.entryList();
    sizeFiles = listProgression.size();
    for( i = 0; i < sizeFiles; i++ )
    {
        if( listProgression.at(i) != "." && listProgression.at(i) != ".." && listProgression.at(i).toInt() != 0 )
            this->readProgression( listProgression.at(i).toInt() );
    }
}


void EngineTvshow::parseLine( char *buffer, qint64 read )
{
    bool sentence = false;
    unsigned int i = 0, pos = 0;
    QString elements[ENGINE_SHOWS_NUMBEROF_ELEMENT];
    while( i < read && buffer[i] != '\n' )
    {
        if( buffer[i] == ' ' && !sentence )
            pos++;
        else if( buffer[i] == '"' )
            sentence = !sentence;
        else if( buffer[i] == ' ' && sentence )
            elements[pos].append( buffer[i] );
        else elements[pos].append( buffer[i] );

        i++;
    }

    if( elements[0].size() > 0 )
        this->tvshows.push_back( new TVShow( elements[3].toInt(), elements[0], elements[1], elements[2].toInt(), elements[4].toInt() ) );
}


int EngineTvshow::countCategories()
{
    this->categories.clear();
    unsigned int i, j, size = this->tvshows.size();
    bool found;
    for( i = 0; i < size; i++ )
    {
        found = false;

        for( j = 0; j < this->categories.size(); j++ )
        {
            if( this->tvshows.at(i)->getCategory() == this->categories.at(j) )
            {
                found = true;
                break;
            }
        }

        if( !found )
            this->categories.push_back( this->tvshows.at(i)->getCategory() );
    }

    return this->categories.size();
}


int EngineTvshow::count( const QString &category ) const
{
    unsigned int i, j = 0, size = this->tvshows.size();
    for( i = 0; i < size; i++ )
    {
        if( category == this->tvshows.at(i)->getCategory() )
            j++;
    }

    return j;
}


unsigned int EngineTvshow::categoryPosition( const QString &name ) const
{
    unsigned int i, size = this->categories.size();
    for( i = 0; i < size; i++ )
    {
        if( name == this->categories.at(i) )
            return i;
    }

    return 0;
}


TVShow* EngineTvshow::get( unsigned int pos, const QString &categoryName )
{
    unsigned int i, size = this->tvshows.size(), counter = 0;
    for( i = 0; i < size; i++ )
    {
        if( this->tvshows.at(i)->getCategory() == categoryName )
        {
            if( counter == pos )
                return this->tvshows.at(i);
            counter++;
        }
    }

    return NULL;
}


void EngineTvshow::save()
{
    QFile file( this->applicationDirPath + FILE_SERIES );
    if( file.open( QIODevice::WriteOnly|QIODevice::Truncate ) )
    {
        unsigned int i, size = this->tvshows.size();
        for( i = 0; i < size; i++ )
        {
            TVShow *tmp = this->tvshows.at(i);
            QString line = QString( "\"%1\" %2 %3 \"%4\" %5 %6 %7\n" ).arg( tmp->getName() ).arg( tmp->getEpisode() ).arg( tmp->getSeason() ).arg( tmp->getCategory() ).arg( tmp->getCheck() ).arg( tmp->getId() ).arg( tmp->getStars() );
            file.write( line.toStdString().c_str() );
        }
        file.close();
    }
    else cout << "ERROR: can't truncate file: " << file.fileName().toStdString() << endl;
}


void EngineTvshow::updateShow( int id )
{
    this->readEpisodes( id );
    this->readDescription( id );
}


void EngineTvshow::readProgression( int id )
{
    TVShow *tvshow = this->getById( id );
    if( tvshow != NULL )
    {
        QFile file( this->applicationDirPath + PATH_SERIES_PROGRESSION + QString::number( id ) );
        if( file.open( QIODevice::ReadOnly ) )
        {
            char buffer[1024];
            int size = sizeof(buffer), read;
            while( ( read = file.readLine( buffer, size ) ) > 0 )
            {
                QString line( buffer );
                QStringList list = line.split('-');
                if( list.size() == 3 )
                {
                    Episode *episode = tvshow->getEpisode( list.at(0).toInt(), list.at(1).toInt() );
                    if( episode != NULL )
                        episode->setSaw( ( list.at(2).toInt() > 0 ) ? true : false );
                    else cout << "WARNING: try to get a null episode: " << list.at(0).toInt() << "-" << list.at(1).toInt() << " in " << file.fileName().toStdString() << endl;
                }
                else cout << "WARNING: wrong number of items in progression file: " << file.fileName().toStdString() << endl;
            }
            file.close();
        }
        else cout << "ERROR: can't open file: " << file.fileName().toStdString() << endl;
    }
}


void EngineTvshow::readDescription( int id )
{
    TVShow *tvshow = this->getById( id );
    if( tvshow != NULL )
    {
        QFile file( this->applicationDirPath + PATH_SERIES_DESCRIPTION + QString::number( id ) );
        if( file.open( QIODevice::ReadOnly ) )
        {
            QDomDocument doc;
            if( doc.setContent( file.readAll() ) )
            {
                file.close();
                QDomNodeList root = doc.elementsByTagName( XML_DESCRIPTION_ROOT );
                if( root.size() > 0 )
                {
                    QDomNodeList nodes = root.at(0).childNodes();
                    int i, size = nodes.size();
                    for( i = 0; i < size; i++ )
                    {
                        if( nodes.at(i).nodeName() == XML_DESCRIPTION_STATUS )
                            tvshow->setStatus( nodes.at(i).toElement().text() );
                        else if( nodes.at(i).nodeName() == XML_DESCRIPTION_STARTED )
                            tvshow->setStarted( nodes.at(i).toElement().text() );
                        else if( nodes.at(i).nodeName() == XML_DESCRIPTION_ENDED )
                        {
                            if( nodes.at(i).toElement().text().length() == 0 )
                                tvshow->setEnded( this->engineLanguage->get( "translation.ended.not" ) );
                            else tvshow->setEnded( nodes.at(i).toElement().text() );
                        }
                        else if( nodes.at(i).nodeName() == XML_DESCRIPTION_COUNTRY )
                            tvshow->setCountry( nodes.at(i).toElement().text() );
                    }
                }
            }
            else cout << "ERROR: can't parse xml file: " << file.fileName().toStdString() << endl;
        }
        else cout << "ERROR: can't open file: " << file.fileName().toStdString() << endl;

        if( !tvshow->isUpToDate() )
            tvshow->setNews( true );
    }
}


void EngineTvshow::readEpisodes( int id )
{
    TVShow *tvshow = this->getById( id );
    if( tvshow != NULL )
    {
        QFile file( this->applicationDirPath + PATH_SERIES_EPISODE + QString::number( id ) );
        if( file.open( QIODevice::ReadOnly ) )
        {
            QDomDocument doc;
            if( doc.setContent( file.readAll() ) )
            {
                file.close();
                QDomNodeList listSeasons = doc.elementsByTagName( XML_SHOW_SEASON ); // Get all seasons
                int i, size = listSeasons.size();
                for( i = 0; i < size; i++ ) // every season
                {
                    QDomNodeList listEpisode = listSeasons.at(i).childNodes();
                    int j, sizeJ = listEpisode.size();
                    for( j = 0; j < sizeJ; j++ ) // every episode
                    {
                        QDomNodeList nodes = listEpisode.at(j).childNodes();
                        QString episodeName;
                        QDate episodeDate;
                        int k, sizeK = nodes.size();
                        for( k = 0; k < sizeK; k++ ) // every attribut tag
                        {
                            //cout << nodes.at(k).nodeName().toStdString() << endl;
                            if( nodes.at(k).nodeName() == XML_EPISODE_NAME )
                                episodeName = nodes.at(k).toElement().text();
                            else if( nodes.at(k).nodeName() == XML_EPISODE_DATE )
                                episodeDate = QDate::fromString( nodes.at(k).toElement().text(), ONLINE_DATE_FORMAT );
                        }
                        tvshow->addEpisode( episodeName, j + 1, i + 1, episodeDate );
                    }
                }
            }
            else cout << "ERROR: can't parse xml file: " << file.fileName().toStdString() << endl;
        }
        else cout << "ERROR: can't open file: " << file.fileName().toStdString() << endl;

        if( !tvshow->isUpToDate())
            tvshow->setNews( true );
    }
}


TVShow* EngineTvshow::getById( int id )
{
    unsigned int i, size = this->tvshows.size();
    for( i = 0; i < size; i++ )
    {
        if( this->tvshows.at(i)->getId() == id )
            return this->tvshows.at(i);
    }

    return NULL;
}
