#include "uishowpanel.h"

UIShowPanel::UIShowPanel( int marginX, int marginY, int width, int height , EngineTvshow *engineTvshow, EnginePicture *enginePicture, EngineProperties *engineProperties, EngineLanguage *engineLanguage, QWidget *parent ) : JLabel( parent)
{
    this->tvshow = NULL;
    this->engineTvshow = engineTvshow;
    this->engineProperties = engineProperties;
    this->enginePicture = enginePicture;
    this->engineLanguage = engineLanguage;
    this->createUI( marginX, marginY, width, height );
}

UIShowPanel::~UIShowPanel()
{      
    delete this->show_status;
    delete this->show_started;
    delete this->show_ended;
    delete this->show_country;
    delete this->optionsPanel;
    delete this->title;
    delete this->categorie;

    int i, size = this->engineProperties->get( "MAX_STARS" ).toInt();
    for( i = 0; i < size; i++ )
        delete this->stars.at(i);

    this->destroyShowUI();

    delete this->moveConteneur;
    delete this->listConteneur;
}


void UIShowPanel::destroyShowUI()
{
    int i, size = this->seasons.size();
    for( i = 0; i < size; i++ )
        delete this->seasons.at(i);
    this->seasons.clear();

    size = this->episodes.size();
    for( i = 0; i < size; i++ )
        delete this->episodes.at(i);
    this->episodes.clear();

    size = this->separators.size();
    for( i = 0; i < size; i++ )
        delete this->separators.at(i);
    this->separators.clear();

    size = this->checks.size();
    for( i = 0; i < size; i++ )
        delete this->checks.at(i);
    this->checks.clear();
}


void UIShowPanel::createUI( int marginX, int marginY, int width, int height )
{
    // Title
    this->title = new QLabel( this );
    this->title->setStyleSheet( "font-size: 30px;" );
    this->title->move( marginX, this->engineProperties->get( "TVSHOW_TITLE_Y_SPACE" ).toInt() );

    // Categorie
    this->categorie = new QLabel( this );
    this->categorie->move( marginX, this->title->pos().y() + this->title->height() + this->engineProperties->get("TVSHOW_DECALLAGE_TITLE_CATEGORIE").toInt( ));

    // Stars
    int i, size = this->engineProperties->get( "MAX_STARS" ).toInt(), starSpace = this->engineProperties->get( "STAR_SPACE" ).toInt();
    const QPixmap *emptyPixmap = this->enginePicture->getPicture( PICTURE_STAR_EMPTY );
    for( i = 0; i < size; i++ )
    {
        JLabel *star = new JLabel( this );
        star->setPixmap( *emptyPixmap );
        star->move( marginX + i * emptyPixmap->width() + starSpace, this->categorie->pos().y() + this->categorie->height() );
        this->connect( star, SIGNAL(mouseEnterEvent()), this, SLOT(changeStarsHover()));
        this->connect( star, SIGNAL(mouseLeaveEvent()), this, SLOT(changeStarsLeave()));
        this->connect( star, SIGNAL(mouseRelease()), this, SLOT(starClicked()));
        this->stars.push_back( star );
    }

    int ySpace = 30;

    // Status
    this->show_status = new QLabel( this );
    this->show_status->move( this->engineProperties->get( "TVSHOW_CALQUE_LEFT" ).toInt(), marginY );
    this->show_status->setStyleSheet( "font-size: 12px" );

    // Started
    this->show_started = new QLabel( this );
    this->show_started->move( this->show_status->pos().x(), marginY + ySpace );
    this->show_started->setStyleSheet( "font-size: 12px" );

    // Ended
    this->show_ended = new QLabel( this );
    this->show_ended->move( this->show_status->pos().x(), marginY + 2 * ySpace );
    this->show_ended->setStyleSheet( "font-size: 12px" );

    // Country
    this->show_country = new QLabel( this );
    this->show_country->move( this->show_status->pos().x(), marginY + 3 * ySpace );
    this->show_country->setStyleSheet( "font-size: 12px" );

    // Episode list
    this->listConteneur = new QWidget( this );
    this->listConteneur->move( this->engineProperties->get( "TVSHOW_CALQUE_LIST_LEFT" ).toInt(), this->engineProperties->get( "TVSHOW_CALQUE_LIST_TOP" ).toInt() );
    this->listConteneur->setFixedSize( width - this->listConteneur->width() - this->engineProperties->get( "TVSHOW_X_DECALLAGE" ).toInt(), height - this->listConteneur->pos().y() - this->engineProperties->get( "TVSHOW_X_DECALLAGE" ).toInt() );
    this->moveConteneur = new QWidget( this->listConteneur );

    // Options panel
    this->optionsPanel = new UIOptionsPanel( this->engineProperties, this->enginePicture, this->engineLanguage, this->engineProperties->get( "OPTIONSPANEL_HEIGHT" ).toInt() - 2 * this->engineProperties->get( "OPTIONSPANEL.MARGIN.Y" ).toInt(), this );
    this->optionsPanel->setNormalPosX( width - this->optionsPanel->getMinLeftWIdth() );
    this->optionsPanel->move( width - this->optionsPanel->getMinLeftWIdth(), this->engineProperties->get( "OPTIONSPANEL_MARGIN_Y" ).toInt() );
}


void UIShowPanel::setTvShow( TVShow *tvshow )
{
    if( tvshow != NULL )
    {
        this->destroyShowUI();

        this->tvshow = tvshow;
        this->show_status->setText( this->engineLanguage->get( "translation.status" ) + " " + tvshow->getStatus() );
        this->show_status->adjustSize();
        this->show_started->setText( this->engineLanguage->get( "translation.started" ) + " " + tvshow->getStarted() );
        this->show_started->adjustSize();
        this->show_ended->setText( this->engineLanguage->get( "translation.ended" ) + " " + tvshow->getEnded() );
        this->show_ended->adjustSize();
        this->show_country->setText( this->engineLanguage->get( "translation.country" ) + " " + tvshow->getCountry() );
        this->show_country->adjustSize();
        this->optionsPanel->setTVShow( tvshow );
        this->title->setText( this->tvshow->getName() );
        this->title->adjustSize();
        this->categorie->setText( this->tvshow->getCategory() );
        int i, size = this->stars.size();
        for( i = 0; i < size; i++ )
        {
            if( i < this->tvshow->getStars() )
                stars.at(i)->setPixmap( *this->enginePicture->getPicture( PICTURE_STAR_FULL ) );
        }

        const vector< vector<Episode*> > *showSeasons = tvshow->getSeasons();
        size = showSeasons->size();
        int top = 0, space = 3;
        for( i = size - 1; i >= 0; i-- )
        {
            QLabel *season = new QLabel( this->engineLanguage->get( "translation.season" ) + " " + QString::number( i + 1 ), this->moveConteneur );
            season->setFixedWidth( 430 );
            season->setStyleSheet( "background-color: #1D1D1D; border-radius: 8px; padding-left: 10px; color: #ECECEC;" );
            season->move( 0, top );

            int j, sizeEpisode = showSeasons->at(i).size();
            for( j = sizeEpisode - 1; j >= 0; j-- )
            {
                Episode *episode = showSeasons->at(i).at(j);

                JLabel *labelEpisode = new JLabel( this->moveConteneur );
                labelEpisode->setText( this->engineLanguage->get( "translation.episode" ) + " " + QString::number( j + 1 ) + ": " + episode->getName() + " - " + episode->getDate().toString() );
                labelEpisode->setStyleSheet( "padding-left: 2px;" );
                labelEpisode->setNormalHoverStyle( "padding-left: 2px;", "background-color: #D1D1D1; border-radius: 5px; padding-left: 2px;" );
                labelEpisode->adjustSize();
                labelEpisode->setFixedWidth( 400 );
                labelEpisode->setToolTip( episode->getName() + " - " + episode->getDate().toString() );

                JLabel *check = NULL;
                if( tvshow->isAfterLast(  i + 1, j + 1  ) != 2 ) // Do we need checkbox ?
                {
                    check = new JLabel( this->moveConteneur );
                    if( tvshow->getEpisode(  i + 1, j + 1  )->isSaw() ) // Episode already saw
                        check->setPixmap( *this->enginePicture->getPicture( PICTURE_CHECK ) );
                    else check->setPixmap( *this->enginePicture->getPicture( PICTURE_UNCHECK ) );
                    this->checks.push_back( check );
                }

                if( j != 0 )
                {
                    QLabel *separator = new QLabel( this->moveConteneur );
                    separator->setStyleSheet( "background-color: #C4C4C4;" );
                    separator->setFixedSize( 400, 1 );
                    this->separators.push_back( separator );
                    top += labelEpisode->height() + separator->height() + space * 2;
                    labelEpisode->move( 20, top );
                    separator->move( 20, labelEpisode->pos().y() + labelEpisode->height() + space );
                }
                else
                {
                    top += labelEpisode->height() + space * 2;
                    labelEpisode->move( 20, top );
                    top += 30;
                }

                if( check != NULL )
                    check->move( 0, labelEpisode->pos().y() );
                this->episodes.push_back( labelEpisode );
            }

            this->seasons.push_back( season );
        }

        this->moveConteneur->adjustSize();
        this->listConteneur->adjustSize();
    }
    else cout << "WARNING: Try to set an NULL show - void UIShowPanel::setTvShow( TVShow *tvshow )" << endl;
}


void UIShowPanel::changeStarsHover()
{
    const QPixmap *fullPixmap = this->enginePicture->getPicture( PICTURE_STAR_FULL );
    const QPixmap *emptyPixmap = this->enginePicture->getPicture( PICTURE_STAR_EMPTY );
    JLabel *senderObject = static_cast<JLabel*>(sender());
    int i, size = this->stars.size();
    bool found = false;
    for( i = 0; i < size; i++ )
    {
        if( found )
            this->stars.at(i)->setPixmap( *emptyPixmap );
        else this->stars.at(i)->setPixmap( *fullPixmap );

        if( senderObject == this->stars.at(i) )
            found = true;
    }
}


void UIShowPanel::changeStarsLeave()
{
    const QPixmap *emptyPixmap = this->enginePicture->getPicture( PICTURE_STAR_EMPTY );
    const QPixmap *fullPixmap = this->enginePicture->getPicture( PICTURE_STAR_FULL );
    int i, size = this->engineProperties->get( "MAX_STARS" ).toInt();
    for( i = 0; i < size; i++ )
    {
        if( i < this->tvshow->getStars() )
            this->stars.at(i)->setPixmap( *fullPixmap );
        else this->stars.at(i)->setPixmap( *emptyPixmap );
    }
}


void UIShowPanel::starClicked()
{
    JLabel *senderObject = static_cast<JLabel*>(sender());
    int i, size = this->stars.size();
    for( i = 0; i < size; i++ )
    {
        if( senderObject == this->stars.at(i) )
        {
            this->tvshow->setStars(i+1);
            this->engineTvshow->save();
            break;
        }
    }
}


void UIShowPanel::wheelTo(float pourcentage)
{
    int ajustement = ceil( this->getListHeight() - this->getListHeightVisible() ) * pourcentage / 100;
    this->moveConteneur->move( this->moveConteneur->pos().x(), -ajustement );
}
