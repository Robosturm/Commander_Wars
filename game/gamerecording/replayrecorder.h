#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QBuffer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class GameMap;
using spGameMap = oxygine::intrusive_ptr<GameMap>;
class GameAction;
using spGameAction = oxygine::intrusive_ptr<GameAction>;

class ReplayRecorder : public QObject
{
    Q_OBJECT
public:
    explicit ReplayRecorder(GameMap* pMap);
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
    bool loadRecord(const QString & filename, spGameMap & pMap);
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
        return m_mods;
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
        return m_progress;
    }
    /**
     * @brief getRecordSize
     * @return
     */
    qint32 getRecordSize()
    {
        return m_count;
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

private:
    QFile m_recordFile;
    QDataStream m_stream{&m_recordFile};
    QStringList m_mods;
    qint32 m_count = 0;
    qint32 m_progress = 0;
    qint64 m_countPos = 0;
    qint64 m_mapPos = 0;
    qint64 m_dailyMapPos = 0;
    qint64 m_posOfDailyMapPos = 0;

    bool m_recording {false};
    bool m_playing{false};
    QBuffer m_dailyBuffer;
    QDataStream m_dailyStream{&m_dailyBuffer};
    qint32 m_currentDay{0};
    GameMap* m_pMap{nullptr};
};

#endif // REPLAYRECORDER_H
