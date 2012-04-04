#include "picture.h"

Picture::Picture(  const string &name, QPixmap pixmap )
{
    this->name = name;
    this->pixmap = pixmap;

   // if( pixmap.isNull() )
       // cout << "ALERT: pixmap named: " << name << " can't be loaded." << endl;
}
