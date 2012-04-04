#ifndef EPISODE_H
#define EPISODE_H

#include <QString>
#include <QDate>

class Episode
{
    public:
        Episode( const QString &name, unsigned int number, unsigned int season, const QDate &date );

        QString getName() const { return this->name; }
        unsigned int getNumber() const { return this->number; }
        unsigned int getSeason() const { return this->season; }
        QDate getDate() const { return this->date; }

        void setSaw( bool value ) { this->saw = value; }

        bool isSaw() const { return this->saw; }

    private:
        QString name;
        unsigned int number;
        unsigned int season;
        QDate date;
        bool saw;
};

#endif // EPISODE_H
