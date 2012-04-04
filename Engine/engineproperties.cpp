#include "engineproperties.h"

EngineProperties::EngineProperties( const string &applicationDirPath )
{
    this->applicationDirPath = applicationDirPath;
    this->collectProperties( this->generalHash, FILE_PROPERTIES );
    this->collectProperties( this->userHash, FILE_USER_PROPERTIES );
}


void EngineProperties::collectProperties( QHash<QString, QString> &hash, QString fileName )
{
    QFile file( ( applicationDirPath + PATH_PROPERTIES + fileName.toStdString() ).c_str() );
    if( file.open( QIODevice::ReadOnly ) )
    {
        char buffer[1024];
        int size = sizeof(buffer), read, pos;
        while( ( read = file.readLine( buffer, size ) ) > 0 )
        {
            pos = 0;
            QString part1, part2;
            bool side = false;

            // Get first part before '='
            while( pos < read )
            {
                if( buffer[pos] == '=' )
                    side = true;
                else
                {
                    if( side )
                        part2.append( buffer[pos] );
                    else part1.append( buffer[pos] );
                }

                pos++;
            }

            if( part1.size() > 0 && part2.size() > 0 )
            {
                if( part2.at( part2.size() - 1 ) == '\n' ) // a tester
                    part2 = part2.left( part2.size() - 1 );
                hash[part1] = part2;
            }
        }
        file.close();
    }
    else cout << "ERROR: can't open file: " << file.fileName().toStdString() << endl;
}


QString EngineProperties::get( const QString &property ) const
{
    return this->generalHash[property];
}


QString EngineProperties::getUser( const QString &property ) const
{
    return this->userHash[property];
}


void EngineProperties::setData( const QString &name, const QVariant &value )
{
    // Change in cache
    this->userHash[name] = value.toString();

    // Change in user data file
    this->setNewValue( FILE_USER_PROPERTIES, this->userHash );
}


void EngineProperties::setProperties( const QString &name, const QVariant &value )
{
    // Change in cache
    this->generalHash[name] = value.toString();

    // Change in user data file
    this->setNewValue( FILE_PROPERTIES, this->generalHash );
}


void EngineProperties::setNewValue( const string &fileName, const QHash<QString, QString> &hash )
{
    QFile file( ( this->applicationDirPath + PATH_PROPERTIES + fileName ).c_str() );
    if( file.open( QIODevice::WriteOnly|QIODevice::Truncate ) )
    {
        QList<QString> keys = hash.keys();
        unsigned int i, size = keys.size();
        for( i = 0; i < size; i++ )
        {
            QString line = QString( "%1=%2\n" ).arg( keys.at(i) ).arg( hash[keys.at(i)] );
            file.write( line.toStdString().c_str() );
        }
        file.close();
    }
    else cout << "ERROR: can't truncate file: " << file.fileName().toStdString() << endl;
}
