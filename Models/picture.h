#ifndef PICTURE_H
#define PICTURE_H

#include <QPixmap>

#include <string>
#include <iostream>

using namespace std;

class Picture
{
    public:
        Picture( const string &name, QPixmap pixmap );

        string getName() const { return this->name; }
        QPixmap* getPicture() { return &this->pixmap; } // no const it's just a helper class

    private:
        string name;
        QPixmap pixmap;
};

#endif // PICTURE_H
