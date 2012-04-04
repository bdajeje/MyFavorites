#ifndef ENGINEINFORMATION_H
#define ENGINEINFORMATION_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
//#include <QQueue>

#include "Defines/define.h"
#include "Engine/enginelanguage.h"
#include "Engine/engineproperties.h"
#include "Models/information.h"
#include "Defines/enum.h"

#include <iostream>

using namespace std;

class EngineInformation : public QWidget
{
    Q_OBJECT

    public:
        explicit EngineInformation( EngineProperties *engineProperties, EngineLanguage *engineLanguage, int width, int height, QWidget *parent = 0 );
        ~EngineInformation();

        void newInformation( int type, const QString name );
        void endInformation();

   // private:
      //  void showInformation();

    private:
        //QQueue<Information> informations;
        EngineProperties *engineProperties;
        EngineLanguage *engineLanguage;        
        QProgressBar *progressBar;
        QLabel *labelText;
};

#endif // ENGINEINFORMATION_H
