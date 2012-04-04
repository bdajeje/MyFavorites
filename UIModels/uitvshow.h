#ifndef UITVSHOW_H
#define UITVSHOW_H

#include <QLabel>
#include <QString>
#include <QTimer>
#include <QMouseEvent>

#include <math.h>

#include "Defines/define_picture.h"
#include "Engine/enginepicture.h"
#include "Engine/enginelanguage.h"
#include "Engine/engineproperties.h"
#include "UIModels/jlabel.h"
#include "Models/tvshow.h"

class UITvshow : public JLabel
{
    Q_OBJECT

    public:
        explicit UITvshow( TVShow *model, EngineLanguage *engineLanguage, EngineProperties *engineProperties, EnginePicture *enginePicture, QWidget *parent );
        ~UITvshow();

        TVShow* model() { return this->_model; }
        void hideName() { this->labelText->hide(); }
        void showName() { this->labelText->show(); }
        void setEnableOver( bool value ) { this->enableOver = value; }
        void setNormalPos( int x, int y ) { this->normalX = x; this->normalY = y; }

        int getNormalX() const { return this->normalX; }
        int getNormalY() const { return this->normalY; }

        bool isInScreen( int maxWidth, int maxHeight, bool hideIt = false );

    private:
        void createUI();        

    private:        
        TVShow *_model;
        QTimer *timerAnimation;                
        int id;
        int normalX;
        int normalY;
        double animMoveRemainX;
        double animMoveRemainY;
        bool animMoveProrato;        
        bool enableOver;
        EnginePicture *enginePicture;
        EngineProperties *engineProperties;
        EngineLanguage *engineLanguage;
        JLabel *labelText;
        QLabel *picture;

    public slots:
        void setNews();

    signals:
        void imNew();
};

#endif // UITVSHOW_H
