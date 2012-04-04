#include "jlabel.h"

JLabel::JLabel( QWidget *parent ) : QLabel(parent)
{
    this->animColorSpeed = 15;
    this->toColor = true;
    this->animOverColorRepeat = false;
    this->stepColor = 10;
    this->timerSpeed = 30;
    this->effect_opactity = NULL;
    this->timerAnimationOpacity = NULL;
    this->isWheelEvent = false;
    this->receiveEventClick = true;
    this->receiveEventHover = true;
    this->isOver = false;
    this->enableOverColor = false;
    this->cursorChange = true;
    this->animation = new QPropertyAnimation( this, "geometry" );
    this->timerOverColor = new QTimer( this );
    this->connect( this->timerOverColor, SIGNAL(timeout()), this, SLOT(timerColorUpdate()));
}


JLabel::~JLabel()
{
    delete this->timerOverColor;
    delete this->animation;

    if( this->timerAnimationOpacity != NULL )
    {
        delete this->timerAnimationOpacity;
        delete this->effect_opactity;
    }
}


void JLabel::enterEvent( QEvent * event )
{
    if( this->receiveEventHover )
    {
        event->accept();
        emit this->mouseEnterEvent();
        this->hovered();
    }
    else event->ignore();
}


void JLabel::hovered()
{
    this->isOver = true;

    if( this->cursorChange )
        this->setCursor(Qt::PointingHandCursor);

    if( this->enableOverColor )
    {
        this->toColor = true;
        this->animOverColor( false );
    }

    if( this->hoverStyle.length() > 0 )
        this->setStyleSheet( this->hoverStyle );

    if( !overPixmap.isNull() )
    {
        if( this->normalPixmap.isNull() )
            this->normalPixmap = this->pixmap()->copy();
        this->setPixmap( this->overPixmap );
    }
}


void JLabel::setNormalHoverStyle( const QString &normalStyle, const QString &hoverStyle )
{
    this->normalStyle = normalStyle;
    this->hoverStyle = hoverStyle;
}


void JLabel::leaveEvent( QEvent * event )
{
    if( this->receiveEventHover )
    {
        event->accept();
        emit this->mouseLeaveEvent();
        this->leaved();
    }
    else event->ignore();
}


void JLabel::leaved()
{
    this->isOver = false;
    if( this->cursorChange )
        this->setCursor( Qt::ArrowCursor );

    if( this->enableOverColor )
    {
        this->toColor = false;
        this->animOverColor( false );
    }

    if( this->normalStyle.length() > 0 )
        this->setStyleSheet( this->normalStyle );

    if( !overPixmap.isNull() )
        this->setPixmap( this->normalPixmap );
}


void JLabel::animOverColor( bool repeat )
{
    this->animOverColorRepeat = repeat;
    if( !this->timerOverColor->isActive() )
        this->timerOverColor->start( this->animColorSpeed );
}

/*
void JLabel::stopAnimOverColor()
{
    this->leaved();
}
*/

void JLabel::mousePressEvent( QMouseEvent * event )
{
    if( this->receiveEventClick )
    {
        event->accept();
        emit this->mouseClick(event);
    }
    else event->ignore();
}


void JLabel::mouseReleaseEvent( QMouseEvent * event )
{
    if( this->receiveEventClick )
    {
        event->accept();
        emit this->mouseRelease();
    }
    else event->ignore();
}


void JLabel::wheelEvent(QWheelEvent *event)
{
    if( this->isWheelEvent )
    {
        event->accept();
        emit mouseWheelEvent(event);
    }
    else event->ignore();
}


void JLabel::animMoveTo( int x, int y, int duration, int width, int height )
{    
    width = ( width >= 0 ) ? width: this->width();
    height = ( height >= 0 ) ? height: this->height();

    this->animation->setDuration( duration );
    //this->animation->setStartValue( QRect( this->pos().x(), this->pos().y(), this->width(), this->height() ));
    this->animation->setEndValue( QRect( x, y, width, height ) );
    this->animation->start();
}


void JLabel::animOpacity( const double &to, int duration, bool hideAtEnd )
{
    this->animOpacityTo = to;
    this->hideAtEnd = hideAtEnd;
    if( this->effect_opactity == NULL )
        this->initEffectOpacity( 1.0 );

    if( this->effect_opactity->opacity() < to )
        this->animOpacityDirection = false;
    else this->animOpacityDirection = true;

    this->timerAnimationOpacity->start( duration );
}


void JLabel::setOpacity( double opacity )
{
    if( this->effect_opactity == NULL )
        this->initEffectOpacity( opacity );
    this->effect_opactity->setOpacity( opacity );
}


void JLabel::initEffectOpacity( double opacity )
{
    this->effect_opactity = new QGraphicsOpacityEffect( this );
    this->effect_opactity->setOpacity( opacity );
    this->setGraphicsEffect( this->effect_opactity );
    this->timerAnimationOpacity = new QTimer( this );
    this->connect( this->timerAnimationOpacity, SIGNAL(timeout()), this, SLOT(animationOpacity()) );
}


void JLabel::animationOpacity()
{
    if( this->animOpacityDirection )
        this->setOpacity( this->effect_opactity->opacity() - 0.1 );
    else this->setOpacity( this->effect_opactity->opacity() + 0.1 );
//cout << this->effect_opactity->opacity() << endl;
    float currentOpacity = this->effect_opactity->opacity();
    float targetOpacity = this->animOpacityTo;

    if( ( this->animOpacityDirection && currentOpacity <= targetOpacity ) || ( !this->animOpacityDirection && currentOpacity >= targetOpacity ) )
    {
        if( this->hideAtEnd )
            this->hide();
        this->timerAnimationOpacity->stop();
      //  cout << "stoped " << this->animOpacityDirection << endl;
    }
}

/*
void JLabel::animOpacityTo( qreal endOpacity, int duration )
{
    this->animationOpacity->setDuration( duration );
    this->animationOpacity->setEndValue( endOpacity );
    this->animationOpacity->start();
}
*/

void JLabel::timerColorUpdate()
{
    QColor color;
    if( this->toColor )
        color = this->overColor;
    else color = this->baseColor;

    this->updateInt( this->red, color.red(), this->stepColor );
    this->updateInt( this->green, color.green(), this->stepColor );
    this->updateInt( this->blue, color.blue(), this->stepColor );

    this->setColor();
    if( this->red != color.red() || this->green != color.green() || this->blue != color.blue() )
    {

    }
    else if( !this->animOverColorRepeat )
        this->timerOverColor->stop();
    else if( this->animOverColorRepeat )    
        this->toColor = !this->toColor;
}


bool JLabel::updateInt( int &base, int target, int step )
{
    bool changed = false;

    if( target != base )
    {
        changed = true;
        if( target < base )
        {
            base -= step;
            if( base < target )
                base = target;
        }
        else
        {
            base += step;
            if( base > target )
                base = target;
        }
    }

    return changed;
}


void JLabel::setBaseColor( QColor color )
{
    this->baseColor = color;
    this->red = color.red();
    this->green = color.green();
    this->blue = color.blue();
    this->setColor();
}


void JLabel::setColor()
{
    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( this->red, this->green, this->blue ) );
    this->setPalette( palette );
}
