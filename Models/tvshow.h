#ifndef TVSHOW_H
#define TVSHOW_H

#include <QString>
#include <QObject>

#include <iostream>
#include <vector>

#include "Models/episode.h"
#include "Defines/online.h"

using namespace std;

class TVShow : public QObject
{
    Q_OBJECT

    public:
        TVShow();
        TVShow( int id, const QString &name, const QString &category, bool checkable, int stars );
        ~TVShow();

        int getId() const { return this->id; }
        int getStars() const { return this->stars; }
        const QString& getName() const { return this->name; }
        const QString& getCategory() const { return this->category; }
        int getCheck() const { return (this->checkable) ? 1 : 0; }
        int getEpisode() const { return this->episode; }
        int getSeason() const { return this->season; }
        const QString& getStatus() const { return this->status; }
        const QString& getStarted() const { return this->started; }
        const QString& getEnded() const { return this->ended; }
        const QString& getCountry() const { return this->country; }
        const vector< vector<Episode*> >* getSeasons() const { return &this->seasons; }
        Episode* getEpisode( unsigned int season, unsigned int episode );

        bool isNews() const { return this->news; }
        bool isUpToDate();
        int isAfterCurrent( unsigned int season, unsigned int episode ); // return true if this season/episode couple are after current season/episode couple
        int isAfterLast( unsigned int season, unsigned int episode ) const; // return true if this season/episode couple are after the last in date season/episode couple

        void addEpisode( const QString &name, unsigned int number, unsigned int season, const QDate &date );

        void setStars( int stars ) { this->stars = stars; }
        void setId( int value ) { this->id = value; }
        void setName( const QString &value ) { this->name = value; }
        void setCategory( const QString &value ) { this->category = value; }
        void setCurrentEpisode( int value ) { this->episode = value; }
        void setCurrentSeason( int value ) { this->season = value; }
        void setCheckable( bool value ) { this->checkable = value; }
        void setNews( bool value ) { if( !this->news && value ) emit gotNews(); this->news = value; }
        void setStatus( const QString &value ) { this->status = value; }
        void setStarted( const QString &value ) { this->started = value; }
        void setEnded( const QString &value ) { this->ended = value; }
        void setCountry( const QString &value ) { this->country = value; }

    private:        
        const Episode* getLastInDate() const;
        int compare( const Episode *episode, unsigned int season, unsigned int episodeNumber ) const;

    private:
        bool news;
        int id;
        QString name;        
        QString category;
        QString status;
        QString started;
        QString ended;
        QString country;
        unsigned int episode;
        unsigned int season;
        bool checkable;
        int stars;
        vector< vector<Episode*> > seasons;        

    signals:
        void gotNews();
};

#endif // TVSHOW_H
