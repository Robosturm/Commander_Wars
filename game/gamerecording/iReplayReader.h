#pragma once

#include <QStringList>

class GameAction;
using spGameAction = std::shared_ptr<GameAction>;
class IReplayReader;
using spIReplayReader = std::shared_ptr<IReplayReader>;

class IReplayReader
{
public:
    virtual bool loadRecord(const QString & filename) = 0;
    virtual QStringList getMods() = 0;
    virtual spGameAction nextAction() = 0;
    virtual qint32 getRecordSize() = 0;
    virtual qint32 getProgess() = 0;
    virtual qint32 getDayFromPosition(qint32 count) = 0;
    virtual void seekToDay(qint32 day) = 0;
};
