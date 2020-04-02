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

    void startRecording();
    void recordAction(GameAction* pAction);
signals:

private:
    QFile m_recordFile;
    QDataStream m_stream{&m_recordFile};
    bool m_recording {false};
};

#endif // REPLAYRECORDER_H
