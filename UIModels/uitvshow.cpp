#include "uitvshow.h"

UITvshow::UITvshow( TVShow *model, EngineLanguage *engineLanguage, EngineProperties *engineProperties, EnginePicture *enginePicture, QWidget *parent ) : JLabel( parent)
{    
    this->engineLanguage = engineLanguage;
    this->enableOver = true;
    this->_model = model;    
    this->timerAnimation = NULL;        
    this->enginePicture = enginePicture;
    this->engineProperties = engineProperties;        

    this->createUI();
    this->setNews();
    QString name = this->_model->getName();
    this->setToolTip( name.append("\n").append( this->engineLanguage->get( "tooltip.show" ) ).append( " ").append( this->engineLanguage->get( "translation.episode" ) ).append( " " ).append( QString::number( _model->getEpisode() ) ).append( " " ).append( this->engineLanguage->get( "translation.season" ) ).append( " " ).append( QString::number( _model->getSeason() ) ) );

    this->connect( this->_model, SIGNAL(gotNews()), this, SLOT(setNews()) );
}


UITvshow::~UITvshow()
{
    delete this->picture;
    delete this->labelText;
    if( this->timerAnimation != NULL )
        delete this->timerAnimation;    
}


void UITvshow::createUI()
{            
    int picturewidth = this->engineProperties->get( "TVSHOW_PICTURE_X" ).toInt(), pictureHeight = this->engineProperties->get( "TVSHOW_PICTURE_Y" ).toInt();

    this->labelText = new JLabel( this );
    this->labelText->setAnimColorSpeed( this->engineProperties->get( "TVSHOW_ANIM_COLOR_SPEED" ).toInt() );
    this->labelText->setReceiveEventHover( false );
    int maxCharacter = 17;
    if( this->_model->getName().length() > maxCharacter )
        this->labelText->setText( this->_model->getName().left(maxCharacter-2).append("..") );
    else this->labelText->setText( this->_model->getName() );
    this->labelText->setStyleSheet( "font-weight: bold;" );
    this->labelText->adjustSize();    
    this->labelText->move( ( picturewidth - this->labelText->width() ) / 2, 0 );        

    this->picture = new QLabel( this );    
    const QPixmap *pixmapPicture = this->enginePicture->getPicture( QString( "%1%2.jpg" ).arg( PICTURES_SERIES ).arg( this->_model->getId() ).toStdString() );    
    if( pixmapPicture != NULL )
        this->picture->setPixmap( pixmapPicture->scaled( picturewidth, pictureHeight ) );
    else this->picture->setPixmap( *this->enginePicture->getPicture( PICTURE_TVSHOW_NOPICTURE ) );

    this->picture->move( 0, this->labelText->height() + 5 );

    this->setFixedSize( picturewidth, this->picture->pos().y() + pictureHeight - this->labelText->pos().y() );
    this->setNormalHoverStyle( " ", "background-color: #B9B9B9; border-radius: 15px;" );
}


bool UITvshow::isInScreen( int maxWidth, int maxHeight, bool hideIt )
{
    if( this->pos().x() + this->width() > 0 && this->pos().x() < maxWidth && this->pos().y() + this->height() > 0 && this->pos().y() < maxHeight )            
        return true;    
    else
    {
        if( hideIt )
            this->hide();
        return false;
    }
}


void UITvshow::setNews()
{
    if( this->model()->isNews() )
    {
       emit imNew();
       this->labelText->setBaseColor( QColor( 100,70,50 ) );
       this->labelText->setOverColor( QColor( 190,160,10 ) );
       this->labelText->animOverColor( true );
       //this->setPixmap( *this->enginePicture->getPicture( PICTURE_TVSHOW_BACKGROUND_NEWS ) );
    }
    else this->animOverColorRepeat = false;
    //else this->setPixmap( *this->enginePicture->getPicture( PICTURE_TVSHOW_BACKGROUND ) );
}

