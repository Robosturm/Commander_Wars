#ifndef REPLAYRECORDER_H
#define REPLAYRECORDER_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QBuffer>

class GameMap;
using spGameMap = std::shared_ptr<GameMap>;
class GameAction;
using spGameAction = std::shared_ptr<GameAction>;

class ReplayRecorder final : public QObject
{
    Q_OBJECT
    enum class Type
    {
        Action,
        Map,
    };

    struct HeaderInfo
    {
        HeaderInfo(Type type)
            : m_type(type)
        {
        }
        HeaderInfo(QDataStream & stream)
        {
            stream >> m_version;
            qint32 type;
            stream >> type;
            m_type = static_cast<Type>(type);
            stream >> m_nextSeekPos;
        }
        qint32 m_version{1};
        Type m_type{Type::Action};
        qint64 m_nextSeekPos{0};
    };
    static const qint32 VERSION = 2;
public:
    explicit ReplayRecorder(GameMap* pMap);
    virtual ~ReplayRecorder();
    /**
     * @brief startRecording
     */
    void startRecording(const QString & file = "");
    /**
     * @brief recordAction
     * @param pAction
     */
    void recordAction(const spGameAction & action);
    /**
     * @brief continueRecording
     * @param file
     */
    bool continueRecording(const QString & file);
    /**
     * @brief loadRecord
     * @param filename
     */
    bool loadRecord(const QString & filename);
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
    qint32 getCount() const;


    QString getRecordJson() const;

private:
    bool validRecord(QByteArray & envData);
    void writeAction(const spGameAction & action);
    void writeMapState();
    ReplayRecorder::HeaderInfo seekToNextType(Type type, bool & success);
    QString createRecordJson() const;
private:
    QFile m_recordFile;
    QDataStream m_stream{&m_recordFile};
    QStringList m_mods;
    QString m_recordJson;
    qint32 m_count = 0;
    qint32 m_progress = 0;
    qint64 m_countPos = 0;
    qint64 m_mapPos = 0;
    qint64 m_streamStart = 0;

    bool m_recording {false};
    bool m_playing{false};
    qint32 m_currentDay{0};
    GameMap* m_pMap{nullptr};
};

#endif // REPLAYRECORDER_H
