#include "Util/UtilString.h"

bool UtilString::isStringFinishBy( const QString &sentence, const QString &looked )
{
    bool result = true;

    if( sentence.length() >= looked.length() )
    {
        int i, size = looked.length();
        for( i = 0; i < size; i++ )
        {
            if( sentence.at( sentence.length() - 1 - i ) != looked.at( looked.length() - 1 - i ) )
            {
                result = false;
                break;
            }
        }
    }
    else result = false;

    return result;
}


//return sentence cut by the end until we found occurence time character
QString UtilString::removeToBeginning( const QString &sentence, char character, unsigned int occurence )
{
    QString result = "";
    unsigned int found = 0;

    for( int i = sentence.length() - 1; i > 0; i-- )
    {
        if( found == occurence )
        {
            result = sentence.left(  i + 1 );
            break;
        }
        else if( sentence.at(i) == character )
            found++;
    }

    return result;
}
