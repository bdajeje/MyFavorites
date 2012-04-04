#ifndef UISCROLLBAR_H
#define UISCROLLBAR_H

#include <QWidget>

#include <iostream>

#include "Defines/define.h"
#include "Defines/define_picture.h"
#include "Engine/enginepicture.h"
#include "Engine/engineproperties.h"
#include "UIModels/jlabel.h"

using namespace std;

class UIScrollbar : public QWidget
{
    Q_OBJECT

    public:
        explicit UIScrollbar( const EngineProperties *engineProperties, EnginePicture *enginePicture, int height, QWidget *referer, QWidget *parent );
        ~UIScrollbar();

        void drawMid( float size );
        void mouseMoved( int y );
        void resetPos();

        int getHeight() const { return this->height() - this->top->height() - this->bot->height() + this->engineProperties->get( "SCROLLBAR_ADJUST_TOP_HEIGHT" ).toInt() + this->engineProperties->get( "SCROLLBAR_ADJUST_BOT_HEIGHT" ).toInt(); }

        void setWheelStep( int value ) { this->wheelStep = value; }

        void wheelEvent(QWheelEvent *);

    private:
        void createUI( int height );
        void moveScrollbar( int x, int y, bool emitMovement = true );
        int getScrollbarHeight();

    private:
        int wheelStep;
        int hitPoint;
        EnginePicture *enginePicture;
        const EngineProperties *engineProperties;
        QWidget *referer;
        QLabel *cover;
        JLabel *top;
        JLabel *mid_top;
        JLabel *mid_mid;
        JLabel *mid_bot;
        JLabel *bot;

    signals:
        void scrollBarActive();
        void scrollBarInactive();
        void scrollBarMoved( float pourcentage );

    public slots:
        void moveSrollbar(QMouseEvent *event);
        void stopMovingScrollbar();
        void moveSrollbarTop();
        void moveSrollbarBot();
        void createWheelEvent(QWheelEvent *event);
};

#endif // UISCROLLBAR_H
