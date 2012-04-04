#ifndef ENGINELANGUAGE_H
#define ENGINELANGUAGE_H

#include <QHash>
#include <QFile>
#include <QDir>
#include <QStringList>

#include <iostream>

#include "Defines/define.h"

using namespace std;

class EngineLanguage
{
    public:
        EngineLanguage( const string &applicationDirPath, const QString &actualLanguage );

        QString get( const QString &sentence ) const;

    private:
        void collectProperties( const string &applicationDirPath ); // This function loads every available languages because we want to hot change languages

    private:
        QString actualLanguage;
        QHash< QString, QHash<QString, QString> > hash;
};

#endif // ENGINELANGUAGE_H
