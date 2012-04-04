#ifndef ENGINEPROPERTIES_H
#define ENGINEPROPERTIES_H

#include <QHash>
#include <QFile>
#include <QStringList>
#include <QVariant>

#include <iostream>

#include "Defines/define.h"

using namespace std;

class EngineProperties
{
    public:
        EngineProperties( const string &applicationDirPath );

        QString get( const QString &property ) const;
        QString getUser( const QString &property ) const;

        void setProperties( const QString &name, const QVariant &value );
        void setData( const QString &name, const QVariant &value );

    private:
        void collectProperties( QHash<QString, QString> &hash, QString fileName );
        void setNewValue( const string &fileName, const QHash<QString, QString> &hash );

    private:
        string applicationDirPath;
        QHash<QString, QString> generalHash;
        QHash<QString, QString> userHash;
};

#endif // ENGINEPROPERTIES_H
