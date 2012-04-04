#include "episode.h"

Episode::Episode( const QString &name, unsigned int number, unsigned int season, const QDate &date )
{
    this->name = name;
    this->number = number;
    this->season = season;
    this->date = date;
}
