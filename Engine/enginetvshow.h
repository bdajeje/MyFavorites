#ifndef ENGINETVSHOW_H
#define ENGINETVSHOW_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QDomDocument>
#include <QDir>
#include <QDate>

#include <vector>
#include <iostream>

#include "Models/tvshow.h"
#include "Defines/define.h"
#include "Defines/xml.h"
#include "Engine/enginelanguage.h"

using namespace std;

class EngineTvshow : public QObject
{
    Q_OBJECT

    public:
        EngineTvshow( const EngineLanguage *engineLanguage, const QString &applicationDirPath );
        ~EngineTvshow();

        TVShow* get( unsigned int pos, const QString &categoryName );
        TVShow* getById( int id );
        int countCategories();
        int count( const QString &category ) const;// numbers of tvshow in a category
        int countAll() const { return this->tvshows.size(); }
        unsigned int categoryPosition( const QString &name ) const;
        const QString getCategory( int pos ) const { return this->categories.at(pos); }

        void save();

    private:
        void initializeSeries();
        void parseLine( char *buffer, qint64 read );
        void readEpisodes( int id );
        void readDescription( int id );
        void readProgression( int id );

    private:
        const EngineLanguage *engineLanguage;
        QString applicationDirPath;
        vector<TVShow*> tvshows;
        vector<QString> categories;

    public slots:
        void updateShow( int id );
};

#endif // ENGINETVSHOW_H
