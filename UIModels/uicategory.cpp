#include "uicategory.h"

UICategory::UICategory( EnginePicture *enginePicture, const QString &name, QWidget *parent ) : JLabel( parent )
{    
    this->newsBackground = NULL;
    this->newsNumber = NULL;

    this->enginePicture = enginePicture;
    this->totalNews = 0;
    this->name = name;        

    this->setFixedHeight( parent->height() );
    this->createUI( name );        
}


UICategory::~UICategory()
{
    if( this->newsBackground != NULL )
    {
        delete this->newsNumber;
        delete this->newsBackground;
    }    
}


void UICategory::createUI( const QString &name )
{    
    this->setText( name );
    this->adjustSize();
}


void UICategory::addNews()
{    
    this->totalNews++;
    this->drawNews();
}


void UICategory::drawNews()
{
    if( this->newsBackground == NULL )
    {
        this->newsBackground = new QLabel( this );        
        this->enginePicture->scalePicture( PICTURE_NEWS_BACKGROUND, 12, 12 );
        this->newsBackground->setPixmap( *this->enginePicture->getPicture( PICTURE_NEWS_BACKGROUND ) );
        this->newsBackground->move( this->width() - this->newsBackground->pixmap()->width(), 1 );
        this->newsNumber = new QLabel( this->newsBackground );        
        this->newsNumber->setStyleSheet( "font-size: 10px; color: black;" );
    }

    if( this->totalNews > 0 )
    {        
        this->newsNumber->setText( QString::number( this->totalNews ) );
        this->newsNumber->adjustSize();
        this->newsNumber->move( ( this->newsBackground->pixmap()->width() - this->newsNumber->width() ) / 2, ( this->newsBackground->pixmap()->height() - this->newsNumber->height() ) / 2 );
        this->newsBackground->show();
    }
    else this->newsBackground->hide();
}
