#include "enginelanguage.h"

EngineLanguage::EngineLanguage( const string &applicationDirPath, const QString &actualLanguage )
{
    this->actualLanguage = actualLanguage;
    this->collectProperties( applicationDirPath );
}

void EngineLanguage::collectProperties( const string &applicationDirPath )
{
    QDir directory( ( applicationDirPath + PATH_LANGUAGES ).c_str() );
    QStringList list = directory.entryList();
    int i, sizeFiles = list.size();
    for( i = 0; i < sizeFiles; i++ )
    {        
        if( list.at(i) != "." && list.at(i) != ".." )
        {
            QFile file( directory.absolutePath() + "/" + list.at(i) + "/translation" );
            if( file.open( QIODevice::ReadOnly ) )
            {
                QHash<QString,QString> translations;
                QString languageName = list.at(i);
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
                        if( part2.at( part2.size()-1 ) == '\n' )
                            part2 = part2.left( part2.size() -  1);
                        translations[part1] = part2;
                    }
                }
                file.close();
                this->hash[languageName] = translations;                
            }
            else cout << "ERROR: can't open file: " << file.fileName().toStdString() << endl;
        }
    }
}


QString EngineLanguage::get( const QString &sentence ) const
{
    return (this->hash[this->actualLanguage])[sentence];
}
