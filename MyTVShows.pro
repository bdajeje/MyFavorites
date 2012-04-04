#-------------------------------------------------
#
# Project created by QtCreator 2012-03-11T12:43:09
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = MyTVShows
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Models/tvshow.cpp \
    Engine/enginepicture.cpp \
    Models/picture.cpp \
    Engine/enginetvshow.cpp \
    UIModels/uicategory.cpp \
    UIModels/uitvshow.cpp \
    UIModels/jlabel.cpp \
    UIModels/uiscrollbar.cpp \
    Engine/engineproperties.cpp \
    UIModels/uishowpanel.cpp \
    Engine/enginelanguage.cpp \
    UIModels/uioptionspanel.cpp \
    Engine/engineonline.cpp \
    Engine/enginecron.cpp \
    Models/request.cpp \
    Engine/engineinformation.cpp \
    Models/information.cpp \
    Models/episode.cpp \
    Util/UtilString.cpp

HEADERS  += mainwindow.h \
    Models/tvshow.h \
    Defines/define.h \
    Engine/enginepicture.h \
    Models/picture.h \
    Defines/define_picture.h \
    Engine/enginetvshow.h \
    UIModels/uicategory.h \
    UIModels/uitvshow.h \
    UIModels/jlabel.h \
    UIModels/uiscrollbar.h \
    Engine/engineproperties.h \
    UIModels/uishowpanel.h \
    Engine/enginelanguage.h \
    UIModels/uioptionspanel.h \
    Engine/engineonline.h \
    Defines/online.h \
    Engine/enginecron.h \
    Models/request.h \
    Defines/xml.h \
    Engine/engineinformation.h \
    Models/information.h \
    Defines/enum.h \
    Models/episode.h \
    Defines/Util.h \
    Util/UtilString.h
