#include "request.h"

Request::Request( int type, QVariant value, const QString &name )
{
    this->type = type;
    this->value = value;
    this->name = name;
}
