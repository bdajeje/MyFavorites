#ifndef UICATEGORY_H
#define UICATEGORY_H

#include <QLabel>
#include <QTimer>
#include <QMouseEvent>

#include "Defines/define.h"
#include "Defines/define_picture.h"
#include "Engine/enginepicture.h"
#include "UIModels/jlabel.h"

#include <iostream>

#include "Engine/engineproperties.h"

using namespace std;

class UICategory : public JLabel
{
    Q_OBJECT

    public:
        UICategory( EnginePicture *enginePicture, const QString &name, QWidget *parent );
        ~UICategory();

        const QString& getName() const { return this->name; }               

    private:
        void createUI( const QString &name );        
        void drawNews();

    private:        
        EnginePicture *enginePicture;        
        int totalNews;
        QString name;        

        QLabel *newsBackground;
        QLabel *newsNumber;

    public slots:        
        void addNews();
};

#endif // UICATEGORY_H

