#ifndef JLABEL_H
#define JLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

#include <iostream>

#include "Engine/engineproperties.h"

using namespace std;

class JLabel : public QLabel
{
    Q_OBJECT

    public:
        explicit JLabel( QWidget *parent = NULL );
        ~JLabel();

        void setOverPixmap( const QPixmap pixmap ) { this->overPixmap = pixmap; }
        void setCursorChange( bool value ) { this->cursorChange = value; }
        void setOpacity( double opacity );
        void setReceiveEventHover( bool value ) { this->receiveEventHover = value; }
        void setReceiveEventClick( bool value ) { this->receiveEventClick = value; }
        void setBaseColor( QColor color );
        void setOverColor( QColor color ) { this->enableOverColor = true; this->overColor = color; }
        void setNormalPos( int x, int y ) { this->normalX = x; this->normalY = y; }
        void setAnimColorSpeed( int speed ) { this->animColorSpeed = speed; }
        void setNormalHoverStyle( const QString &normalStyle, const QString &hoverStyle );

        void enableWheelEvent( bool value ) { this->isWheelEvent = value; }

        int getNormalX() const { return this->normalX; }
        int getNormalY() const { return this->normalY; }

        void enterEvent( QEvent * event );
        void leaveEvent( QEvent * event );
        void mousePressEvent( QMouseEvent * event );
        void mouseReleaseEvent( QMouseEvent * event );
        void wheelEvent(QWheelEvent *event);

        void animMoveTo( int x, int y, int duration, int width = -1, int height = -1 );
        void animOpacity( const double &to, int duration, bool setHideAtEnd = true );
        void animOverColor( bool repeat );

    protected:
        void initEffectOpacity( double opacity );
        void setColor();
        bool updateInt( int &base, int target, int step );        

    protected:
        QString normalStyle;
        QString hoverStyle;
        int animColorSpeed;
        QTimer *timerAnimationOpacity;
        QGraphicsOpacityEffect *effect_opactity;
        double animOpacityTo;
        bool enableOverColor;
        bool hideAtEnd;
        bool isWheelEvent;
        bool cursorChange;
        bool animOpacityDirection;
        bool isOver;
        bool receiveEventHover;
        bool receiveEventClick;
        QPixmap normalPixmap;
        QPixmap overPixmap;
        QPropertyAnimation *animation;
        int red;
        int green;
        int blue;
        int stepColor;
        int timerSpeed;
        bool animOverColorRepeat;
        bool toColor;
        QTimer *timerOverColor;
        QColor baseColor;
        QColor overColor;
        int normalX;
        int normalY;

    signals:
        void mouseClick(QMouseEvent * event);
        void mouseRelease();
        void mouseEnterEvent();
        void mouseLeaveEvent();
        void mouseWheelEvent(QWheelEvent *);

    public slots:
        void timerColorUpdate();
        void hovered();
        void leaved();
        void animationOpacity();
};

#endif // JLABEL_H
