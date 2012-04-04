#include "enginepicture.h"

EnginePicture::EnginePicture( const string &applicationDirPath )
{
    this->applicationDirPath = applicationDirPath;
}


const QPixmap* EnginePicture::getPicture( const string &name )
{
    return this->getEditablePicture( name );
}


QPixmap* EnginePicture::getEditablePicture( const string &name )
{
    int i, size = this->pictures.size();
    for( i = 0; i < size; i++ )
    {
        if( this->pictures.at(i).getName() == name )
            return this->pictures.at(i).getPicture();
    }

    QPixmap pixmap = QPixmap::fromImage( QImage ( QString( this->applicationDirPath.c_str() ).append( PATH_PICTURES ).append( name.c_str() ) ) );
    this->pictures.push_back( Picture( name, pixmap ) );
    if( pixmap.isNull() )
    {
        cout << "WARNING: can't load QPixmap: " << name << endl;
        return NULL;
    }
    else return this->pictures.at( this->pictures.size() - 1 ).getPicture();
}


void EnginePicture::scalePicture( const string &name, int width, int height )
{
    QPixmap *tmp = this->getEditablePicture( name );
    *tmp = tmp->scaled( width, height );
}
