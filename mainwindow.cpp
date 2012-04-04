#include "mainwindow.h"

enum STEP{ STEP_TVSHOWS, STEP_TVSHOW };

MainWindow::MainWindow( const QString &applicationDirPath, QWidget *parent ) : QMainWindow( parent )
{    
    this->setAttribute( Qt::WA_AlwaysShowToolTips );
    this->setStyleSheet( "font-size: 14px;" );

    this->step = STEP_TVSHOWS;
    this->sendMouseMoveMsg = false;
    this->currentShow = NULL;
    this->oldCategory = this->currentCategory = NULL;
    this->applicationDirPath = applicationDirPath + "/";

    this->engineProperties = new EngineProperties( this->applicationDirPath.toStdString() );
    this->engineLanguage = new EngineLanguage( this->applicationDirPath.toStdString(), this->engineProperties->getUser( "LANGUAGE_ACTUAL" ) );
    this->enginePicture = new EnginePicture( this->applicationDirPath.toStdString() );
    this->engineTvshow = new EngineTvshow( this->engineLanguage, this->applicationDirPath );

    // Do it just one time in the entire programm
    int checkBoxWidth = this->engineProperties->get( "PICTURE_CHECKBOX_WIDTH" ).toInt(), checkBoxHeight = this->engineProperties->get( "PICTURE_CHECKBOX_HEIGHT" ).toInt();
    this->enginePicture->scalePicture( PICTURE_CHECK, checkBoxWidth, checkBoxHeight );
    this->enginePicture->scalePicture( PICTURE_UNCHECK, checkBoxWidth, checkBoxHeight );
    this->enginePicture->scalePicture( PICTURE_TVSHOW_NOPICTURE, this->engineProperties->get( "TVSHOW_PICTURE_X" ).toInt(), this->engineProperties->get( "TVSHOW_PICTURE_Y" ).toInt() );

    this->createUI();    

    int engineInformationHeight = this->engineProperties->get( "ENGINE_INFORMATION_HEIGHT" ).toInt();
    this->engineInformation = new EngineInformation( this->engineProperties, this->engineLanguage, this->engineProperties->get( "ENGINE_INFORMATION_WIDTH" ).toInt(), engineInformationHeight, this->mainWidget );
    this->engineInformation->move( this->engineProperties->get( "ENGINE_INFORMATION_X" ).toInt(), this->height() - engineInformationHeight - this->engineProperties->get( "ENGINE_INFORMATION_MARGIN_BOT" ).toInt() );

    this->engineOnline = new EngineOnline( this->applicationDirPath, this->engineInformation );
    this->connect( this->engineOnline, SIGNAL(showUpdated(int)), this->engineTvshow, SLOT(updateShow(int)) );

    this->engineCron = new EngineCron( this->engineOnline, this->engineProperties, this->engineTvshow );
    this->connect( this->button_checkOnline, SIGNAL(mouseRelease()), this->engineCron, SLOT(clickUpdate()) );
}

MainWindow::~MainWindow()
{    
    int i, size = this->categories.size();
    for( i = 0; i < size; i++ )
        delete this->categories.at(i);

    size = this->tvshows.size();
    for( i = 0; i < size; i++ )
        delete this->tvshows.at(i);    

    delete this->button_options;
    delete this->button_checkOnline;
    delete this->button_backToCategories;

    delete this->labelName;
    delete this->scrollbar;
    delete this->categoriesOver;
    delete this->tvshowsCalque;
    delete this->tvshowCalque;    
    delete this->categoriesField;
    delete this->tvshowsField;
    delete this->engineInformation;
    delete this->mainWidget;
    delete this->engineCron;
    delete this->engineOnline;
    delete this->enginePicture;
    delete this->engineTvshow;
    delete this->engineProperties;    
    delete this->engineLanguage;
}


void MainWindow::createUI()
{    
    this->setFixedSize( WINDOW_WIDTH, WINDOW_HEIGHT );

    // App background
    this->mainWidget = new QLabel( this );
    this->mainWidget->setPixmap( *this->enginePicture->getPicture( PICTURE_APPLICATION_BACKGROUND ) );

    // App name
    this->labelName = new QLabel( this->engineProperties->get( "APPLICATION_NAME" ), this->mainWidget );
    this->labelName->setStyleSheet( "font-size: 30px;" );
    this->labelName->move( this->engineProperties->get( "LABELNAME_X_DECALLAGE" ).toInt(), this->engineProperties->get( "LABELNAME_Y_DECALLAGE" ).toInt() );

    // Main right buttons
    this->button_options = new JLabel( this );
    this->enginePicture->scalePicture( PICTURE_BUTTON_OPTIONS, 30, 30 );
    this->button_options->setPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_OPTIONS ) );
    this->enginePicture->scalePicture( PICTURE_BUTTON_OPTIONS_HOVER, 30, 30 );
    this->button_options->setOverPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_OPTIONS_HOVER ) );
    this->button_options->adjustSize();
    this->button_options->setToolTip( this->engineLanguage->get( "tooltip.button.options" ) );
    this->button_options->move( this->width() - this->button_options->width() - this->engineProperties->get( "BUTTONS_MARGIN_RIGHT" ).toInt(), this->engineProperties->get( "BUTTONS_MARGIN_TOP" ).toInt() );

    this->button_checkOnline = new JLabel( this );
    this->enginePicture->scalePicture( PICTURE_BUTTON_CHECKONLINE, 30, 30 );
    this->button_checkOnline->setPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_CHECKONLINE ) );
    this->enginePicture->scalePicture( PICTURE_BUTTON_CHECKONLINE_HOVER, 30, 30 );
    this->button_checkOnline->setOverPixmap( *this->enginePicture->getPicture( PICTURE_BUTTON_CHECKONLINE_HOVER ) );
    this->button_checkOnline->adjustSize();
    this->button_checkOnline->setToolTip( this->engineLanguage->get( "tooltip.button.checkOnline" ) );
    this->button_checkOnline->move( this->button_options->pos().x() - this->button_checkOnline->width() - this->engineProperties->get( "BUTTONS_SPACE" ).toInt(), this->button_options->pos().y() );    

    // Categories field
    this->categoriesField = new QLabel( this->mainWidget );
    const QPixmap *categoriesFieldPixmap = this->enginePicture->getPicture( PICTURE_CATEGORY_BACKGROUND );
    this->categoriesField->setFixedSize( categoriesFieldPixmap->width(), categoriesFieldPixmap->height() );
    this->categoriesField->setPixmap( *categoriesFieldPixmap );
    this->categoriesField->move( ( WINDOW_WIDTH - categoriesFieldPixmap->width() ) / 2, this->labelName->pos().y() + this->labelName->height() + this->engineProperties->get( "CATEGORY_Y_DECALLAGE" ).toInt() );

    // Categories Over
    this->categoriesOver = new JLabel( this->categoriesField );
    this->categoriesOver->setStyleSheet("background-color: #7F7F7F; border-radius: 10px;");

    // TV Shows field
    this->tvshowsField = new QLabel( this->mainWidget );
    const QPixmap *tvshowFieldPixmap = this->enginePicture->getPicture( PICTURE_TVSHOWS_BACKGROUND );
    this->tvshowsField->setFixedSize( tvshowFieldPixmap->width(), tvshowFieldPixmap->height() );
    this->tvshowsField->setPixmap( *tvshowFieldPixmap );
    this->tvshowsField->move( this->engineProperties->get( "TVSHOWFIELD_MARGIN_X" ).toInt(), WINDOW_HEIGHT - tvshowFieldPixmap->height() - 50 );

    // TV Shows calque pour la scroll bar
    this->tvshowsCalque = new JLabel( this->tvshowsField );
    this->tvshowsCalque->setFixedSize( this->tvshowsField->width() - 2 * this->engineProperties->get( "CALQUE_MARGIN_X" ).toInt(), this->tvshowsField->height() - this->engineProperties->get( "CALQUE_HEIGHT_REDUCTION" ).toInt() );
    this->tvshowsCalque->move( this->engineProperties->get( "CALQUE_MARGIN_X" ).toInt(), this->engineProperties->get( "CALQUE_POS_Y" ).toInt() );
    this->tvshowsCalque->enableWheelEvent( true );
    this->tvshowsCalque->setCursorChange( false );

    // Button back to categories
    this->button_backToCategories = new JLabel( this->categoriesField );
    this->button_backToCategories->setBaseColor( QColor( 10,0,0 ) );
    this->button_backToCategories->setOverColor( QColor( 255,255,255 ) );
    this->button_backToCategories->setText( this->engineLanguage->get( "main.backToCategories" ) );
    this->button_backToCategories->adjustSize();
    this->button_backToCategories->move( -this->button_backToCategories->width(), ( this->categoriesField->height() - this->button_backToCategories->height() ) / 2 );
    this->connect( this->button_backToCategories, SIGNAL(mouseRelease()), this, SLOT(backToCategoriesClicked()) );

    // Categories list    
    int i, size = this->engineTvshow->countCategories(), x = this->engineProperties->get( "CATEGORY_X_DECALLAGE" ).toInt();
    for( i = 0; i < size; i++ )
    {        
        UICategory *category = new UICategory( this->enginePicture, this->engineTvshow->getCategory(i), this->categoriesField );
        category->setBaseColor( QColor( 10,0,0 ) );
        category->setOverColor( QColor( 255,255,255 ) );
        category->move( x, ( this->categoriesField->height() - category->height() ) / 2 );
        category->setNormalPos( category->pos().x(), category->pos().y() );
        this->connect( category, SIGNAL(mouseRelease()), this, SLOT(categoryClicked()) );
        this->categories.push_back( category );
        x += category->width() + this->engineProperties->get( "CATEGORY_X_SPACE" ).toInt();

        // TV Show list
        int row = 0, column = 0, j;
        int sizeShows = this->engineTvshow->count( this->engineTvshow->getCategory(i) ), normalX, normalY;
        for( j = 0; j < sizeShows; j++ )
        {
            TVShow *tvshow = this->engineTvshow->get( j, category->getName() );
            UITvshow *uitvshow = new UITvshow( tvshow, this->engineLanguage, this->engineProperties, this->enginePicture, this->tvshowsCalque );

            this->connect( uitvshow, SIGNAL(mouseRelease()), this, SLOT(showTvshow()) );
            this->connect( uitvshow, SIGNAL(imNew()), category, SLOT(addNews()) );

            if( tvshow->isNews() )
                category->addNews();

            normalX = this->engineProperties->get( "TVSHOW_X_DECALLAGE" ).toInt() + column * ( uitvshow->width() + this->engineProperties->get( "TVSHOW_X_SPACE" ).toInt() );
            normalY = this->engineProperties->get( "TVSHOW_Y_DECALLAGE" ).toInt() + row * ( uitvshow->height() + this->engineProperties->get( "TVSHOW_Y_SPACE" ).toInt() );
            uitvshow->setNormalPos( normalX, normalY );

            if( i == 0 )
                uitvshow->move( normalX, normalY );
            else uitvshow->hide();
            this->tvshows.push_back( uitvshow );

            column++;
            if( column == this->engineProperties->get( "TVSHOW_NUMBERPERROW" ).toInt() )
            {
                row++;
                column = 0;
            }
        }
    }        

    if( !this->categories.empty() )
    {
        this->currentCategory = this->categories.at(0);

        int space = this->engineProperties->get( "CATEGORY_X_SPACEWITHOVER" ).toInt();
        this->categoriesOver->animMoveTo( this->currentCategory->pos().x() - space, this->currentCategory->pos().y() + 5, this->engineProperties->get( "ANIMATIONS_SPEED" ).toInt(), this->currentCategory->width() + 2 * space );
    }

    // TV Show calque pour une seule série
    this->tvshowCalque = new UIShowPanel( this->engineProperties->get( "TVSHOW_DECALLAGE_PICTURE" ).toInt() + this->engineProperties->get( "TVSHOW_X_DECALLAGE" ).toInt() + this->tvshows.at(0)->width(), this->tvshows.at(0)->height(), this->tvshowsField->width(), this->tvshowsField->height(), this->engineTvshow, this->enginePicture, this->engineProperties, this->engineLanguage, this->tvshowsField );
    this->tvshowCalque->enableWheelEvent( true );
    this->tvshowCalque->setFixedSize( this->tvshowsField->width() - this->engineProperties->get( "CALQUE_MARGIN_X" ).toInt() - this->engineProperties->get( "OPTIONSPANEL.MARGIN.RIGHT" ).toInt(), this->tvshowsField->height() - this->engineProperties->get( "CALQUE_HEIGHT_REDUCTION" ).toInt() );
    this->tvshowCalque->move( this->engineProperties->get( "CALQUE_MARGIN_X" ).toInt(), this->engineProperties->get( "CALQUE_POS_Y" ).toInt() );
    this->tvshowCalque->setCursorChange( false );
    this->tvshowCalque->hide();

    // Scrollbar
    this->scrollbar = new UIScrollbar( this->engineProperties, this->enginePicture, this->tvshowsCalque->height(), this->tvshowsCalque, this->mainWidget );
    this->scrollbar->drawMid( this->calculateScrollBarHeight( this->tvshowsCalque->height(), this->getTvshowsHeight() ) );
    this->scrollbar->move( this->width() - this->scrollbar->width() - this->engineProperties->get( "SCROLLBAR_MARGIN_X" ).toInt(), this->tvshowsField->pos().y() + ( this->tvshowsField->height() - this->scrollbar->height() ) / 2 + this->engineProperties->get( "SCROLLBAR_MARGIN_Y" ).toInt() );
    this->connect( this->scrollbar, SIGNAL(scrollBarActive()), this, SLOT(setScrollbarActive()) );
    this->connect( this->scrollbar, SIGNAL(scrollBarInactive()), this, SLOT(setScrollbarInactive()) );
    this->connect( this->scrollbar, SIGNAL(scrollBarMoved(float)), this, SLOT(scrollBarMoved(float)));

    this->connect( this->tvshowsCalque, SIGNAL(mouseWheelEvent(QWheelEvent *)), this->scrollbar, SLOT(createWheelEvent(QWheelEvent *)));
    this->connect( this->tvshowCalque, SIGNAL(mouseWheelEvent(QWheelEvent *)), this->scrollbar, SLOT(createWheelEvent(QWheelEvent *)));

    this->setCentralWidget( this->mainWidget );    
}


int MainWindow::calculateScrollBarHeight( float min, float max )
{
    return (float)this->scrollbar->getHeight() * (float)( min / max );
}


int MainWindow::getTvshowsHeight()
{
    if( !this->tvshows.empty() )
    {
        int row = ceil( (float)this->engineTvshow->count(this->currentCategory->getName()) / this->engineProperties->get( "TVSHOW_NUMBERPERROW" ).toFloat() );
        int rowHeight = ( this->tvshows.at(0)->height() + this->engineProperties->get( "TVSHOW_Y_SPACE").toFloat() );
        return row * rowHeight;
    }
    else return 0;
}

void MainWindow::categoryClicked()
{
    this->oldCategory = this->currentCategory;
    this->currentCategory = static_cast<UICategory*>( this->sender() );
    this->scrollbar->resetPos();
    this->scrollbar->drawMid( this->calculateScrollBarHeight(this->tvshowsCalque->height(), this->getTvshowsHeight()) );
    this->initializeCategoryAnimPosition( this->getCategoryAnimSide() );
    this->animationCategory();  
    this->categoriesOver->animMoveTo( this->currentCategory->pos().x() - this->engineProperties->get( "CATEGORY_X_SPACEWITHOVER" ).toInt(), this->categoriesOver->pos().y(), this->engineProperties->get( "ANIMATIONS_SPEED" ).toInt(), this->currentCategory->width() + 2 * this->engineProperties->get( "CATEGORY_X_SPACEWITHOVER" ).toInt() );
}


// right = true / left = false;
void MainWindow::initializeCategoryAnimPosition( bool side )
{
    unsigned int i, size = this->tvshows.size(), column = 0, row = 0;
    int decallageX;
    if( side )
        decallageX = this->tvshowsCalque->width() + this->engineProperties->get( "TVSHOW_X_SPACE" ).toInt();
    else decallageX = -this->tvshowsCalque->width() - this->engineProperties->get( "TVSHOW_X_SPACE" ).toInt();

    for( i = 0; i < size; i++ )
    {
        if( this->tvshows.at(i)->model()->getCategory() == this->currentCategory->getName() )
        {
            this->tvshows.at(i)->move( decallageX + this->engineProperties->get( "TVSHOW_X_DECALLAGE" ).toInt() + column * ( this->tvshows.at(i)->width() + this->engineProperties->get( "TVSHOW_X_SPACE" ).toInt() ), this->engineProperties->get( "TVSHOW_Y_DECALLAGE" ).toInt() + row * ( this->tvshows.at(i)->height() + this->engineProperties->get( "TVSHOW_Y_SPACE" ).toInt() ) );
            this->tvshows.at(i)->show();
            column++;
            if( column == this->engineProperties->get( "TVSHOW_NUMBERPERROW" ).toUInt() )
            {
                row++;
                column = 0;
            }
        }
    }
}


// right = true / left = false;
bool MainWindow::getCategoryAnimSide()
{
    if( this->engineTvshow->categoryPosition( this->oldCategory->getName() ) < this->engineTvshow->categoryPosition( this->currentCategory->getName() ) )
        return true;
    else return false;
}


void MainWindow::animationCategory()
{
    // Which way left or right ?
    bool side = this->getCategoryAnimSide();

    // Move tvshows
    unsigned int i, size = this->tvshows.size();
    for( i = 0; i < size; i++ )
    {
        if( this->tvshows.at(i)->model()->getCategory() == this->oldCategory->getName() )
        {  
            if( side )            
                this->tvshows.at(i)->animMoveTo( this->tvshows.at(i)->getNormalX() - this->tvshowsCalque->width(), this->tvshows.at(i)->pos().y(), this->engineProperties->get( "ANIMATIONS_SPEED" ).toInt() );
            else this->tvshows.at(i)->animMoveTo( this->tvshows.at(i)->getNormalX() + this->tvshowsCalque->width(), this->tvshows.at(i)->pos().y(), this->engineProperties->get( "ANIMATIONS_SPEED" ).toInt() );
        }        
        else if( this->tvshows.at(i)->model()->getCategory() == this->currentCategory->getName() )
            this->tvshows.at(i)->animMoveTo( this->tvshows.at(i)->getNormalX(), this->tvshows.at(i)->pos().y(), this->engineProperties->get( "ANIMATIONS_SPEED" ).toInt() );
    }
}


void MainWindow::mouseMoveEvent( QMouseEvent *event )
{
    if( this->sendMouseMoveMsg )
    {
        event->accept();
        this->scrollbar->mouseMoved( event->pos().y() );
    }
}


void MainWindow::setScrollbarActive()
{
    this->sendMouseMoveMsg = true;
}


void MainWindow::setScrollbarInactive()
{
    this->sendMouseMoveMsg = false;
}


void MainWindow::scrollBarMoved( float pourcentage )
{
    if( this->step == STEP_TVSHOWS )
        this->moveCalque( pourcentage );
    else if( this->step == STEP_TVSHOW )
        this->tvshowCalque->wheelTo( pourcentage );
}


void MainWindow::moveCalque( float pourcentage )
{    
    int ajustement = ceil( this->getTvshowsHeight() - this->tvshowsCalque->height() ) * pourcentage / 100, row = 0;
    unsigned int i, size = this->tvshows.size(), decallageY = this->engineProperties->get( "TVSHOW_Y_DECALLAGE" ).toUInt(), ySpace = this->engineProperties->get( "TVSHOW_Y_SPACE" ).toUInt(), rowNumber = this->engineProperties->get( "TVSHOW_NUMBERPERROW" ).toUInt();
    for( i = 0; i < size; i++ )
    {
        if( this->currentCategory->getName() == this->tvshows.at(i)->model()->getCategory() )
        {
            if( i >= rowNumber && i % rowNumber == 0 )
                row++;
            this->tvshows.at(i)->move( this->tvshows.at(i)->pos().x(), decallageY + row * ( this->tvshows.at(i)->height() + ySpace ) - ajustement );
        }
    }
}


void MainWindow::showTvshow()
{    
    if( this->step != STEP_TVSHOW )
    {
        this->step = STEP_TVSHOW;                

        // Retrieve sender object
        this->currentShow = static_cast<UITvshow*>( sender() );
        this->currentShow->setEnableOver( false );
        this->currentShow->setCursor(Qt::ArrowCursor);
        this->currentShow->leaved();

        // Fade away uitvshows
        unsigned int i, size = this->tvshows.size();
        int animationOpacitySpeed = this->engineProperties->get( "ANIMATIONS_OPACITY_SPEED" ).toInt();
        for( i = 0; i < size; i++ )
        {
            if( this->currentCategory->getName() == this->tvshows.at(i)->model()->getCategory() && this->currentShow != this->tvshows.at(i) )
            {
                this->tvshows.at(i)->setOpacity( 1.0 );
                this->tvshows.at(i)->animOpacity( 0.0, animationOpacitySpeed );
            }
        }

        // Move selected uitvshow
        int moveSpeed = this->engineProperties->get( "ANIMATIONS_SPEED" ).toInt();
        this->currentShow->animMoveTo( this->engineProperties->get( "TVSHOW_X_DECALLAGE" ).toInt(), this->engineProperties->get( "TVSHOW_Y_DECALLAGE_CALQUE" ).toInt(), moveSpeed );
        this->currentShow->hideName();

        // Fade in the tv show calque
        this->tvshowCalque->setTvShow( this->currentShow->model() );
        this->tvshowCalque->setOpacity( 0.0 );
        this->tvshowCalque->show();
        this->tvshowCalque->animOpacity( 1.0, animationOpacitySpeed, false );

        this->scrollbar->setWheelStep(2);
        this->scrollbar->resetPos();
        this->scrollbar->drawMid( this->calculateScrollBarHeight( this->tvshowCalque->getListHeightVisible(), this->tvshowCalque->getListHeight() ) );

        // Move away categories and move in "back to categories"
        size = this->categories.size();
        int xMove = -this->categories.at( this->categories.size() - 1 )->pos().x() - this->categories.at(0)->width();
        for( i = 0; i < size; i++ )
            this->categories.at(i)->animMoveTo( xMove, this->categories.at(i)->pos().y(), moveSpeed );
        this->categoriesOver->animMoveTo( xMove, this->categoriesOver->pos().y(), moveSpeed );
        this->button_backToCategories->animMoveTo( this->engineProperties->get( "CATEGORY_X_DECALLAGE" ).toInt(), this->button_backToCategories->pos().y(), moveSpeed );
    }
}


void MainWindow::backToCategoriesClicked()
{
    if( this->step != STEP_TVSHOWS )
    {
        this->step = STEP_TVSHOWS;

        this->scrollbar->setWheelStep(5);
        this->scrollbar->resetPos();
        this->scrollbar->drawMid( this->calculateScrollBarHeight( this->tvshowsCalque->height(), this->getTvshowsHeight() ) );
        //this->moveCalque( 0.0 ); // replace uitvshow to normal places

        // Fade in uitvshows
        unsigned int i, size = this->tvshows.size();
        int animationOpacitySpeed = this->engineProperties->get( "ANIMATIONS_OPACITY_SPEED" ).toInt();
        for( i = 0; i < size; i++ )
        {
            if( this->currentCategory->getName() == this->tvshows.at(i)->model()->getCategory() && this->currentShow != this->tvshows.at(i) )
            {
                this->tvshows.at(i)->show();
                this->tvshows.at(i)->setOpacity( 0.0 );
                this->tvshows.at(i)->animOpacity( 1.0, animationOpacitySpeed, false );
            }
        }

        // Move current uitvshow
        int moveSpeed = this->engineProperties->get( "ANIMATIONS_SPEED" ).toInt();
        this->currentShow->animMoveTo( this->currentShow->getNormalX(), this->currentShow->getNormalY(), moveSpeed );
        this->currentShow->showName();

        // Fade away the tv show calque
        this->tvshowCalque->animOpacity( 0.0, animationOpacitySpeed, true );

        // Move in categories and move away "back to categories"
        size = this->categories.size();
        for( i = 0; i < size; i++ )
            this->categories.at(i)->animMoveTo( this->categories.at(i)->getNormalX(), this->categories.at(i)->getNormalY(), moveSpeed );
        this->categoriesOver->animMoveTo( this->currentCategory->getNormalX() - this->engineProperties->get( "CATEGORY_X_SPACEWITHOVER" ).toInt(), this->categoriesOver->pos().y(), moveSpeed );
        this->button_backToCategories->animMoveTo( -this->button_backToCategories->width(), this->button_backToCategories->pos().y(), moveSpeed );
    }
}
