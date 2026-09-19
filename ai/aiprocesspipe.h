#ifndef AIPROCESSPIPE_H
#define AIPROCESSPIPE_H

#include <QObject>
#include <QProcess>

#include "network/localclient.h"
#include "network/localserver.h"
#include "game/gameanimation/animationskipper.h"

class GameMap;
class GameAction;
using spGameAction = std::shared_ptr<GameAction>;
class GameMenue;
using spGameMenue = std::shared_ptr<GameMenue>;
class AiProcessPipe;
using spAiProcessPipe = std::shared_ptr<AiProcessPipe>;
using spQProcess = std::shared_ptr<QProcess>;

class AiProcessPipe final : public QObject
{
    Q_OBJECT
public:
    enum class PipeState
    {
        Disconnected,
        Connected,
        Ready,
        PreparingGame,
        Ingame,
    };
    explicit AiProcessPipe();
    virtual ~AiProcessPipe();

    void onGameStarted(GameMenue* pMenu);
    void onQuitGame();
    void quit();
    PipeState getPipeState() const;
    void spawnSubProcess();

signals:
    void sigStartPipe();
    void sigPipeReady();
    void sigPerformAction(spGameAction pAction, bool fromAiPipe);
public slots:
    void sendActionToMaster(spGameAction pAction);
    void sendActionToSlave(spGameAction pAction);
    void nextAction();
private slots:
    void startPipe();
    void onConnected(quint64 socket);
    void disconnected(quint64 socket);
    void recieveData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket);
private:
    void onNewActionForMaster(QDataStream & stream);
    void onNewAction(QDataStream & stream);
    void onStartGame(QDataStream & stream);
    void pipeReady();
    void quitGame();
    void performNextBufferedAction(GameMenue* pMenu);
private:
    static constexpr qint32 MaxBufferedActions{200};
    NetworkInterface * m_pActiveConnection{nullptr};
    spLocalServer m_pServer{nullptr};
    spLocalClient m_pClient{nullptr};
    PipeState m_pipeState{PipeState::Disconnected};
    AnimationSkipper m_animationSkipper;
    QList<spGameAction> m_ActionBuffer;
    std::mutex m_ActionMutex;
    GameMap* m_pMap{nullptr};
    std::weak_ptr<oxygine::EventDispatcher> m_pMenu;
    spQProcess m_aiSubProcess;
};

#endif // AIPROCESSPIPE_H
