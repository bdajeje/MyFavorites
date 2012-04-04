#ifndef UITHOWPANEL_H
#define UITHOWPANEL_H

#include <QWidget>

#include <vector>
#include <math.h>
#include <iostream>

#include "Models/tvshow.h"
#include "UIModels/jlabel.h"
#include "Engine/engineproperties.h"
#include "Engine/enginepicture.h"
#include "Engine/enginetvshow.h"
#include "Engine/enginelanguage.h"
#include "UIModels/uioptionspanel.h"
#include "Defines/define_picture.h"

using namespace std;

class UIShowPanel : public JLabel
{
    Q_OBJECT

    public:
        UIShowPanel( int marginX, int marginY, int width, int height , EngineTvshow *engineTvshow, EnginePicture *enginePicture, EngineProperties *engineProperties, EngineLanguage *engineLanguage, QWidget *parent );
        ~UIShowPanel();

        void setTvShow( TVShow *tvshow );

        int getListHeight() const { return this->moveConteneur->height(); }
        int getListHeightVisible() const { return this->height() - this->listConteneur->pos().y(); }

        void wheelTo( float pourcentage );

    private:
        void createUI( int marginX, int marginY, int width, int height );
        void destroyShowUI();

    private:
        EngineTvshow *engineTvshow;
        EngineProperties *engineProperties;
        EnginePicture *enginePicture;
        EngineLanguage *engineLanguage;
        TVShow *tvshow;
        QLabel *title;
        QLabel *categorie;
        vector<JLabel*> stars;
        QLabel *description;
        JLabel *button_options;
        UIOptionsPanel *optionsPanel;
        QLabel *show_status;
        QLabel *show_started;
        QLabel *show_ended;
        QLabel *show_country;
        QWidget *listConteneur;
        QWidget *moveConteneur;
        vector<QLabel*> seasons;
        vector<JLabel*> episodes;
        vector<QLabel*> separators;
        vector<JLabel*> checks;

    public slots:
        void changeStarsHover();
        void changeStarsLeave();
        void starClicked();
};

#endif // UITHOWPANEL_H
