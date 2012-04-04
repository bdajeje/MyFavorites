#ifndef REQUEST_H
#define REQUEST_H

#include <QVariant>
#include <QString>

class Request
{
    public:
        Request( int type, QVariant value, const QString &name );

        int getType() const { return this->type; }
        QVariant getValue() const { return this->value; }
        QString getName() const { return this->name; }

    private:
        QString name;
        int type;
        QVariant value;
};

#endif // REQUEST_H
