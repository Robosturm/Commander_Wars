#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QBuffer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class GameAction;
typedef oxygine::intrusive_ptr<GameAction> spGameAction;

class ReplayRecorder : public QObject
{
    Q_OBJECT
public:
    explicit ReplayRecorder();
    virtual ~ReplayRecorder();
    /**
     * @brief startRecording
     */
    void startRecording();
    /**
     * @brief recordAction
     * @param pAction
     */
    void recordAction(spGameAction pAction);
    /**
     * @brief loadRecord
     * @param filename
     */
    bool loadRecord(QString filename);
    /**
     * @brief nextAction
     */
    spGameAction nextAction();
    /**
     * @brief getMods
     * @return
     */
    QStringList getMods()
    {
        return _mods;
    }
    /**
     * @brief seekToStart
     */
    void seekToStart();
    /**
     * @brief getProgess
     * @return
     */
    qint32 getProgess()
    {
        return _progress;
    }
    /**
     * @brief getRecordSize
     * @return
     */
    qint32 getRecordSize()
    {
        return _count;
    }
    /**
     * @brief getDayFromPosition
     * @param count
     * @return
     */
    qint32 getDayFromPosition(qint32 count);
    /**
     * @brief seekToDay
     * @param day
     * @return
     */
    void seekToDay(qint32 day);
signals:

private:
    QFile m_recordFile;
    QDataStream m_stream{&m_recordFile};
    QStringList _mods;
    qint32 _count = 0;
    qint32 _progress = 0;
    qint64 _countPos = 0;
    qint64 _mapPos = 0;
    qint64 _dailyMapPos = 0;
    qint64 _posOfDailyMapPos = 0;

    bool m_recording {false};
    bool playing{false};
    QBuffer _dailyBuffer;
    QDataStream _dailyStream{&_dailyBuffer};
    qint32 m_currentDay{0};
};

#endif // REPLAYRECORDER_H
