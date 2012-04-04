#ifndef ENGINECRON_H
#define ENGINECRON_H

#include <QWidget>
#include <QDateTime>
#include <QBasicTimer>
#include <QTimerEvent>

#include "Engine/engineonline.h"
#include "Engine/engineproperties.h"
#include "Engine/enginetvshow.h"
#include "Defines/define.h"

class EngineCron : public QWidget
{
    Q_OBJECT

    public:
        EngineCron( EngineOnline *engineOnline, EngineProperties *engineProperties, EngineTvshow *engineTvShow );
        ~EngineCron();

    protected:
        void timerEvent(QTimerEvent *event);

    private:
        void startCron();
        void checkOnline();

    private:
        EngineOnline *engineOnline;
        EngineProperties *engineProperties;
        EngineTvshow *engineTvShow;
        QBasicTimer *timer; // QBasicTimer is faster than QTimer

    public slots:
        void clickUpdate();
};

#endif // ENGINECRON_H
