#ifndef UIOPTIONSPANEL_H
#define UIOPTIONSPANEL_H

#include <QLineEdit>

#include <iostream>

#include "Engine/enginelanguage.h"
#include "Engine/enginepicture.h"
#include "Engine/engineproperties.h"
#include "UIModels/jlabel.h"
#include "Defines/define_picture.h"
#include "Models/tvshow.h"

using namespace std;

class UIOptionsPanel : public JLabel
{
    Q_OBJECT

    public:
        UIOptionsPanel( EngineProperties *engineProperties, EnginePicture *enginePicture, EngineLanguage *engineLanguage, int height, QWidget *parent );
        ~UIOptionsPanel();

        int getMinLeftWIdth() const;
        int getTicketLeftWIdth() const;

        void setTVShow( TVShow *tvshow );
        void setNormalPosX( int value ) { this->normalPosX = value; }

    private:
        void createUI( int height );
        void createUIForm();

    private:
        int normalPosX;
        bool lock;
        TVShow *model;

        EnginePicture *enginePicture;
        EngineLanguage *engineLanguage;
        EngineProperties *engineProperties;

        JLabel *ticket;
        JLabel *ticketLeft;
        JLabel *ticketLeftIcon;
        JLabel *ticketRightText;
        JLabel *ticketRightBackground;
        JLabel *panel;
        JLabel *closeButton;

        QLineEdit *editName;

    public slots:
        void clickIt();
        void hideIt();
        void showIt();
        void raiseIt();
};

#endif // UIOPTIONSPANEL_H
