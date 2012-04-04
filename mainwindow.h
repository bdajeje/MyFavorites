#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QString>
#include <QLabel>
#include <QTimer>

#include <iostream>
#include <math.h>

#include "Defines/define_picture.h"
#include "Engine/enginepicture.h"
#include "Engine/enginetvshow.h"
#include "Engine/engineproperties.h"
#include "Engine/enginelanguage.h"
#include "Engine/engineonline.h"
#include "Engine/enginecron.h"
#include "Engine/engineinformation.h"
#include "UIModels/uicategory.h"
#include "UIModels/uitvshow.h"
#include "UIModels/uiscrollbar.h"
#include "UIModels/uishowpanel.h"

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        MainWindow( const QString &applicationDirPath, QWidget *parent = 0 );
        ~MainWindow();

        void mouseMoveEvent( QMouseEvent *event );

    private:
        void createUI();
        int getTvshowsHeight();
        bool getCategoryAnimSide();
        void initializeCategoryAnimPosition( bool value );
        void animationCategory();
        int calculateScrollBarHeight( float min, float max );
        void moveCalque( float pourcentage );

    private:
        int step;
        UICategory *currentCategory;
        UICategory *oldCategory;
        UITvshow *currentShow;
        bool sendMouseMoveMsg;
        QString applicationDirPath;

        // Engines
        EnginePicture *enginePicture;
        EngineTvshow *engineTvshow;
        EngineProperties *engineProperties;
        EngineLanguage *engineLanguage;
        EngineCron *engineCron;
        EngineOnline *engineOnline;
        EngineInformation *engineInformation;

        // UI part
        QLabel *mainWidget; // mainWidget is the background
        QLabel *labelName;
        QLabel *categoriesField;
        JLabel *categoriesOver;
        vector<UICategory*> categories;
        QLabel *tvshowsField;
        JLabel *tvshowsCalque;
        UIScrollbar *scrollbar;
        vector<UITvshow*> tvshows;
        UIShowPanel *tvshowCalque;
        JLabel *button_options;
        JLabel *button_checkOnline;
        JLabel *button_backToCategories;        

    private slots:
        void categoryClicked();
        void setScrollbarActive();
        void setScrollbarInactive();        
        void scrollBarMoved( float pourcentage );
        void showTvshow();
        void backToCategoriesClicked();
};

#endif // MAINWINDOW_H
