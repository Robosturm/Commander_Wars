#pragma once

#include <QStringList>

class GameAction;
using spGameAction = std::shared_ptr<GameAction>;
class IReplayReader;
using spIReplayReader = std::shared_ptr<IReplayReader>;

class IReplayReader : public QObject
{
    Q_OBJECT
public:
    struct DayInfo
    {
        qint32 day{0};
        qint32 player{0};
    };

    virtual bool loadRecord(const QString & filename) = 0;
    virtual QStringList getMods() = 0;
    virtual spGameAction nextAction() = 0;
    virtual void onPostAction() = 0;
    virtual qint32 getRecordSize() = 0;
    virtual qint32 getProgess() = 0;
    virtual DayInfo getDayFromPosition(qint32 count) = 0;
    virtual void seekToDay(DayInfo day) = 0;
    virtual void requestReplayStart() = 0;
signals:
    void startReplay();
};
