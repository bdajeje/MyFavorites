#ifndef INFORMATION_H
#define INFORMATION_H

#include <QString>

class Information
{
    public:
        Information( int type, const QString &name );

        const QString& getName() const { return this->name; }
        int getType() const { return this->type; }

    private:
        int type;
        QString name;        
};

#endif // INFORMATION_H

