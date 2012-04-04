#include "uioptionspanel.h"

UIOptionsPanel::UIOptionsPanel( EngineProperties *engineProperties, EnginePicture *enginePicture, EngineLanguage *engineLanguage, int height, QWidget *parent ) : JLabel( parent )
{
    this->model = NULL;
    this->lock = false;

    this->enginePicture = enginePicture;
    this->engineLanguage = engineLanguage;
    this->engineProperties = engineProperties;

    this->createUI( height );
    this->createUIForm();
    this->setCursorChange( false );
}


UIOptionsPanel::~UIOptionsPanel()
{
    delete this->ticketLeft;
    delete this->ticketLeftIcon;
    delete this->ticketRightText;
    delete this->ticketRightBackground;
    delete this->closeButton;
    delete this->editName;
    delete this->panel;
    delete this->ticket;
}


void UIOptionsPanel::createUI( int height )
{    
    this->ticket = new JLabel( this );

    this->ticketLeft = new JLabel( this->ticket );
    this->ticketLeft->setPixmap( *this->enginePicture->getPicture( PICTURE_OPTIONSPANEL_LEFT ) );
    this->ticketLeft->setReceiveEventClick( false );
    this->ticketLeft->setReceiveEventHover( false );
    this->ticketLeft->move( 0, 0 );

    this->ticketLeftIcon = new JLabel( this->ticket );
    this->ticketLeftIcon->setPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_OPTIONS ) );
    this->ticketLeftIcon->adjustSize();
    this->ticketLeftIcon->move( this->engineProperties->get( "OPTIONSPANEL_ICON_MARGIN_X" ).toInt(), ( this->ticketLeft->pixmap()->height() - this->ticketLeftIcon->pixmap()->height() ) / 2 );
    this->ticketLeftIcon->setReceiveEventClick( false );
    this->ticketLeftIcon->setReceiveEventHover( false );

    this->ticketRightText = new JLabel();// Set parent after
    this->ticketRightText->setText( this->engineLanguage->get( "optionspanel.title" ) );
    this->ticketRightText->adjustSize();
    this->ticketRightText->setReceiveEventClick( false );
    this->ticketRightText->setReceiveEventHover( false );

    this->ticketRightBackground = new JLabel( this->ticket );
    this->enginePicture->scalePicture( PICTURE_OPTIONSPANEL_MID, this->ticketRightText->width() + 10, this->ticketLeft->pixmap()->height() );
    this->ticketRightBackground->setPixmap( *this->enginePicture->getPicture( PICTURE_OPTIONSPANEL_MID ) );
    this->ticketRightBackground->move( this->ticketLeft->pixmap()->width(), 0 );
    this->ticketRightBackground->setReceiveEventClick( false );
    this->ticketRightBackground->setReceiveEventHover( false );

    this->ticketRightText->setParent( this->ticket );
    this->ticketRightText->move( this->ticketLeftIcon->pos().x() + this->ticketLeftIcon->width() + 10, ( this->ticketRightBackground->pixmap()->height() - this->ticketRightText->height() ) / 2 );

    this->panel = new JLabel( this );
    this->panel->setCursorChange( false );
    this->panel->setFixedSize( this->engineProperties->get( "OPTIONSPANEL_WIDTH" ).toInt(), height );
    this->panel->setPixmap( *this->enginePicture->getPicture( PICTURE_OPTIONSPANEL ) );
    this->panel->move( this->ticketRightBackground->pos().x() + this->ticketRightBackground->pixmap()->width(), 0 );

    this->closeButton = new JLabel( this->panel );
    this->enginePicture->scalePicture( PICTURE_BUTTON_CLOSE, this->engineProperties->get( "PICTURE_CLOSEBUTTON_WIDTH" ).toInt(), this->engineProperties->get( "PICTURE_CLOSEBUTTON_HEIGHT" ).toInt() );
    this->enginePicture->scalePicture( PICTURE_BUTTON_CLOSE_HOVER, this->engineProperties->get( "PICTURE_CLOSEBUTTON_WIDTH" ).toInt(), this->engineProperties->get( "PICTURE_CLOSEBUTTON_HEIGHT" ).toInt() );
    this->closeButton->setPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_CLOSE ) );
    this->closeButton->setOverPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_CLOSE_HOVER ) );
    this->closeButton->move( this->panel->width() - this->closeButton->width() - this->engineProperties->get( "PICTURE_CLOSEBUTTON_MARGIN_RIGHT" ).toInt(), this->engineProperties->get( "PICTURE_CLOSEBUTTON_MARGIN_TOP" ).toInt() );    

    this->ticket->setFixedSize( this->ticketLeft->pixmap()->width() + this->ticketRightBackground->pixmap()->width(), this->ticketLeft->pixmap()->height() );
    this->setFixedSize( this->panel->width() + this->ticket->width(), height );

    this->connect( this->ticket, SIGNAL(mouseEnterEvent()), this, SLOT(raiseIt()));
    this->connect( this->ticket, SIGNAL(mouseLeaveEvent()), this, SLOT(hideIt()));
    this->connect( this->ticket, SIGNAL(mouseRelease()), this, SLOT(clickIt()));
    this->connect( this->closeButton, SIGNAL(mouseRelease()), this, SLOT(clickIt()));
}


void UIOptionsPanel::createUIForm()
{
    this->editName = new QLineEdit( this->panel );
    this->editName->move( this->engineProperties->get( "OPTIONSPANEL_CONTENT_MARGIN_LEFT" ).toInt(), this->engineProperties->get( "OPTIONSPANEL_CONTENT_MARGIN_TOP" ).toInt() );
}


void UIOptionsPanel::setTVShow( TVShow *tvshow )
{
    this->model = tvshow;

    this->editName->setText( this->model->getName() );
}

int UIOptionsPanel::getMinLeftWIdth() const
{
    return this->ticketLeft->pixmap()->width();
}


int UIOptionsPanel::getTicketLeftWIdth() const
{
    return this->ticketLeft->width() + this->ticketRightBackground->width();
}


void UIOptionsPanel::raiseIt()
{
    if( !this->lock )
    {
        this->animMoveTo( this->normalPosX - this->ticketRightBackground->width(), this->pos().y(), this->engineProperties->get( "ANIMATION_TICKETOPTIONS" ).toInt() );
        this->ticketLeftIcon->setPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_OPTIONS_HOVER ) );
    }
}


void UIOptionsPanel::hideIt()
{
    if( !this->lock )
    {
        this->animMoveTo( this->normalPosX, this->pos().y(), this->engineProperties->get( "ANIMATION_TICKETOPTIONS" ).toInt() );
        this->ticketLeftIcon->setPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_OPTIONS ) );
    }
}


void UIOptionsPanel::showIt()
{
    this->lock = true;
    this->animMoveTo( this->normalPosX - this->panel->width(), this->pos().y(), this->engineProperties->get( "ANIMATION_TICKETOPTIONS" ).toInt() );
}


void UIOptionsPanel::clickIt()
{
    if( this->lock )
    {
        this->lock = false;
        this->hideIt();
    }
    else
    {
        this->lock = true;
        this->showIt();
    }
}
