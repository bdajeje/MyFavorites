#ifndef ENGINEPICTURE_H
#define ENGINEPICTURE_H

#include <QPixmap>

#include <vector>
#include <iostream>

#include "Defines/define.h"
#include "Models/picture.h"

using namespace std;

class EnginePicture
{
    public:
        EnginePicture( const string &applicationDirPath );

        const QPixmap* getPicture( const string &name );
        void scalePicture( const string &name, int width, int height );

    private:
        QPixmap* getEditablePicture( const string &name );

    private:
        string applicationDirPath;
        vector<Picture> pictures;
};

#endif // ENGINEPICTURE_H
