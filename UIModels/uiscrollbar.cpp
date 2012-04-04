#include "uiscrollbar.h"

UIScrollbar::UIScrollbar( const EngineProperties *engineProperties, EnginePicture *enginePicture, int height, QWidget *referer, QWidget *parent ) : QWidget(parent)
{
    this->hitPoint = 0;
    this->wheelStep = 5;
    this->referer = referer;
    this->engineProperties = engineProperties;
    this->enginePicture = enginePicture;
    this->createUI( height );
}

UIScrollbar::~UIScrollbar()
{
    delete this->top;
    delete this->mid_bot;
    delete this->mid_top;
    delete this->mid_mid;
    delete this->bot;
    delete this->cover;
}


void UIScrollbar::createUI( int height )
{
    this->setFixedSize( this->engineProperties->get( "SCROLLBAR_WIDTH" ).toInt(), height );

    this->cover = new QLabel( this );
    const QPixmap *pixmap = this->enginePicture->getPicture( PICTURE_SCROLLBAR_BACKGROUND );
    this->cover->setFixedHeight( height );
    this->cover->setPixmap( pixmap->scaled( this->engineProperties->get( "SCROLLBAR_WIDTH" ).toInt(), height ) );

    this->top = new JLabel( this->cover );
    this->top->setPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_TOP ) );
    this->top->setOverPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_TOP_HOVER ) );
    this->top->adjustSize();

    this->mid_top = new JLabel( this->cover );
    this->mid_top->setPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_MID_TOP ) );
    this->mid_top->setOverPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_MID_TOP_HOVER ) );
    this->mid_top->adjustSize();    

    this->mid_bot = new JLabel( this->cover );
    this->mid_bot->setPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_MID_BOT ) );
    this->mid_bot->setOverPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_MID_BOT_HOVER ) );
    this->mid_bot->adjustSize();

    this->mid_mid = new JLabel( this->cover );

    this->bot = new JLabel( this->cover );
    this->bot->setPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_BOT ) );
    this->bot->setOverPixmap( *this->enginePicture->getPicture( PICTURE_SCROLLBAR_BOT_HOVER ) );
    this->bot->adjustSize();
    this->bot->move( 0, this->height() - this->bot->height() );

    this->connect( this->mid_top, SIGNAL(mouseEnterEvent()), this->mid_mid, SLOT(hovered()) );
    this->connect( this->mid_top, SIGNAL(mouseEnterEvent()), this->mid_bot, SLOT(hovered()) );
    this->connect( this->mid_mid, SIGNAL(mouseEnterEvent()), this->mid_top, SLOT(hovered()) );
    this->connect( this->mid_mid, SIGNAL(mouseEnterEvent()), this->mid_bot, SLOT(hovered()) );
    this->connect( this->mid_bot, SIGNAL(mouseEnterEvent()), this->mid_top, SLOT(hovered()) );
    this->connect( this->mid_bot, SIGNAL(mouseEnterEvent()), this->mid_mid, SLOT(hovered()) );
    this->connect( this->mid_top, SIGNAL(mouseLeaveEvent()), this->mid_mid, SLOT(leaved()) );
    this->connect( this->mid_top, SIGNAL(mouseLeaveEvent()), this->mid_bot, SLOT(leaved()) );
    this->connect( this->mid_mid, SIGNAL(mouseLeaveEvent()), this->mid_top, SLOT(leaved()) );
    this->connect( this->mid_mid, SIGNAL(mouseLeaveEvent()), this->mid_bot, SLOT(leaved()) );
    this->connect( this->mid_bot, SIGNAL(mouseLeaveEvent()), this->mid_top, SLOT(leaved()) );
    this->connect( this->mid_bot, SIGNAL(mouseLeaveEvent()), this->mid_mid, SLOT(leaved()) );
    this->connect( this->mid_top, SIGNAL(mouseClick(QMouseEvent*)), this, SLOT(moveSrollbar(QMouseEvent*)) );
    this->connect( this->mid_mid, SIGNAL(mouseClick(QMouseEvent*)), this, SLOT(moveSrollbar(QMouseEvent*)) );
    this->connect( this->mid_bot, SIGNAL(mouseClick(QMouseEvent*)), this, SLOT(moveSrollbar(QMouseEvent*)) );
    this->connect( this->mid_top, SIGNAL(mouseRelease()), this, SLOT(stopMovingScrollbar()) );
    this->connect( this->mid_mid, SIGNAL(mouseRelease()), this, SLOT(stopMovingScrollbar()) );
    this->connect( this->mid_bot, SIGNAL(mouseRelease()), this, SLOT(stopMovingScrollbar()) );
    this->connect( this->top, SIGNAL(mouseRelease()), this, SLOT(moveSrollbarTop()));
    this->connect( this->bot, SIGNAL(mouseRelease()), this, SLOT(moveSrollbarBot()));
}


void UIScrollbar::drawMid( float size )
{    
    if( size > this->getHeight() )
        size = this->getHeight();
    size -= this->mid_top->height() + this->mid_bot->height();

    const QPixmap *pixmap = this->enginePicture->getPicture( PICTURE_SCROLLBAR_MID_MID );
    this->mid_mid->setPixmap( pixmap->scaled( pixmap->width(), size ) );
    const QPixmap *pixmapHover = this->enginePicture->getPicture( PICTURE_SCROLLBAR_MID_MID_HOVER );
    this->mid_mid->setOverPixmap( pixmapHover->scaled( pixmapHover->width(), size ) );

    this->mid_mid->setFixedHeight( size );

    this->moveScrollbar( 0, 0 );
}


void UIScrollbar::moveSrollbar( QMouseEvent *event )
{    
    JLabel *objectSender = static_cast<JLabel*>(sender());
    this->hitPoint = event->pos().y() + this->y() + objectSender->pos().y();
    emit scrollBarActive();
}


void UIScrollbar::stopMovingScrollbar()
{
    emit scrollBarInactive();
}


void UIScrollbar::mouseMoved( int y )
{
    this->moveScrollbar( this->mid_top->pos().x(), y - this->hitPoint + this->mid_top->pos().y() );
    this->hitPoint = y;
}


void UIScrollbar::moveScrollbar( int x, int y, bool emitMovement )
{    
    int adjustTop = this->engineProperties->get( "SCROLLBAR_ADJUST_TOP_HEIGHT" ).toInt();
    int adjustBot = this->engineProperties->get( "SCROLLBAR_ADJUST_BOT_HEIGHT" ).toInt();

    if( y < this->top->height() - adjustTop )
        y = this->top->height() - adjustTop;
    else if( y > this->bot->pos().y() + adjustBot - this->getScrollbarHeight() )
        y = this->bot->pos().y() + adjustBot - this->getScrollbarHeight();

    this->mid_top->move( x, y );
    this->mid_mid->move( x, this->mid_top->pos().y() + this->mid_top->height() );
    this->mid_bot->move( x, this->mid_mid->pos().y() + this->mid_mid->height() );

    float marge = this->getHeight() - this->getScrollbarHeight();
    if( marge > 0 && emitMovement )
        emit scrollBarMoved( 100.0 * (float)(this->mid_mid->y() + this->mid_mid->height()/2 - ( this->top->height() - adjustTop + this->mid_top->height() + this->mid_mid->height()/2 ) ) / marge );
}


int UIScrollbar::getScrollbarHeight()
{
    return this->mid_bot->height() + this->mid_mid->height() + this->mid_top->height();
}


void UIScrollbar::moveSrollbarTop()
{
    this->moveScrollbar( this->mid_top->pos().x(), this->mid_top->pos().y() - 5 * this->getHeight() / 100 );
}


void UIScrollbar::moveSrollbarBot()
{
    this->moveScrollbar( this->mid_top->pos().x(), this->mid_top->pos().y() + 5 * this->getHeight() / 100 );
}


void UIScrollbar::wheelEvent(QWheelEvent *event)
{
    event->accept();
    if( event->delta() < 0 )
        this->moveScrollbar( this->mid_top->pos().x(), this->mid_top->pos().y() + this->wheelStep * this->getHeight() / 100 );
    else this->moveScrollbar( this->mid_top->pos().x(), this->mid_top->pos().y() - this->wheelStep * this->getHeight() / 100 );
}


void UIScrollbar::createWheelEvent(QWheelEvent *event)
{
    this->wheelEvent(event);
}


void UIScrollbar::resetPos()
{
    this->moveScrollbar( this->mid_top->pos().x(), 0 );
}
