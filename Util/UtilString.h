#ifndef UTILSTRING_H
#define UTILSTRING_H

#include <QString>

namespace UtilString
{
    bool isStringFinishBy( const QString &sentence, const QString &looked );
    QString removeToBeginning( const QString &sentence, char character, unsigned int occurence );
}

#endif // UTILSTRING_H
