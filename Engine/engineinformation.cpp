#include "engineinformation.h"

EngineInformation::EngineInformation( EngineProperties *engineProperties, EngineLanguage *engineLanguage, int width, int height, QWidget *parent ) : QWidget(parent)
{
    this->engineLanguage = engineLanguage;
    this->engineProperties = engineProperties;

    this->setFixedSize( width, height );

    this->labelText = new QLabel( this );
    this->labelText->setText( "Default text" );
    this->labelText->adjustSize();
    this->labelText->move( this->engineProperties->get( "ENGINE_INFORMATION_TEXT_X" ).toInt(), ( height - this->labelText->height() ) / 2 );

    this->progressBar = new QProgressBar( this );
    this->progressBar->setMaximum(0);
    this->progressBar->setValue( 0 );
    this->progressBar->setFixedSize( width / 2, height );
    this->progressBar->move( width / 2, 0 );
    this->hide();
}


EngineInformation::~EngineInformation()
{    
    delete this->labelText;
    delete this->progressBar;
}


void EngineInformation::newInformation( int type, const QString name )
{    
    //this->informations.enqueue( Information( type, name ) );
    //this->showInformation();

    this->labelText->setText( name + ": " + this->engineLanguage->get( "informations.type." + QString::number( type ) ) );
    this->labelText->adjustSize();
    this->show();
}


void EngineInformation::endInformation()
{
    //this->informations.dequeue();
    //if( this->informations.empty() )
        this->hide();
    //else this->showInformation();
}

/*
void EngineInformation::showInformation()
{
    if( !this->informations.empty() )
    {
        this->label->setText( this->engineLanguage->get( "informations.type." + QString::number( this->informations.head().getType() ) ) + this->informations.head().getName() );
        this->label->adjustSize();
        this->label->move( ( this->progressBar->width() - this->label->width() ) / 2, ( this->progressBar->height() - this->label->height() ) / 2 );
        this->progressBar->show();
    }
}
*/
