#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QObject>
#include <QFile>
#include <QDataStream>

class GameAction;

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
    void recordAction(GameAction* pAction);
    /**
     * @brief loadRecord
     * @param filename
     */
    bool loadRecord(QString filename);
    /**
     * @brief nextAction
     */
    GameAction* nextAction();
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
signals:

private:
    QFile m_recordFile;
    QDataStream m_stream{&m_recordFile};
    QStringList _mods;
    qint32 _count = 0;
    qint64 _countPos = 0;
    qint64 _mapPos = 0;
    bool m_recording {false};
    bool playing{false};
};

#endif // REPLAYRECORDER_H
