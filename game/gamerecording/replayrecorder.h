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
    void loadRecord(QString filename);
    /**
     * @brief nextAction
     */
    void nextAction();
signals:

private:
    QFile m_recordFile;
    QDataStream m_stream{&m_recordFile};
    bool m_recording {false};
    bool playing{false};
};

#endif // REPLAYRECORDER_H
