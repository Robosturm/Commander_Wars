#ifndef AIPROCESSPIPE_H
#define AIPROCESSPIPE_H

#include <QObject>

#include "network/localclient.h"
#include "network/localserver.h"
#include "game/gameanimation/animationskipper.h"

class GameMap;
class GameAction;
using spGameAction = oxygine::intrusive_ptr<GameAction>;
class GameMenue;
using spGameMenue = oxygine::intrusive_ptr<GameMenue>;
class AiProcessPipe;
using spAiProcessPipe = oxygine::intrusive_ptr<AiProcessPipe>;

class AiProcessPipe final : public QObject, public oxygine::ref_counter
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
    ~AiProcessPipe();

    void onGameStarted(GameMenue* pMenu);
    void onQuitGame();
    void quit();
    PipeState getPipeState() const;


signals:
    void sigStartPipe();
    void sigPipeReady();
    void sigPerformAction(spGameAction pAction);
public slots:
    void sendActionToMaster(spGameAction pAction);
    void sendActionToSlave(spGameAction pAction);
    void nextAction();
private slots:
    void startPipe();
    void onConnected(quint64 socket);
    void disconnected(quint64 socket);
    void recieveData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service);
private:
    void onNewActionForMaster(QDataStream & stream);
    void onNewAction(QDataStream & stream);
    void onStartGame(QDataStream & stream);
    void pipeReady();
    void quitGame();
private:
    NetworkInterface * m_pActiveConnection{nullptr};
    spLocalServer m_pServer{nullptr};
    spLocalClient m_pClient{nullptr};
    PipeState m_pipeState{PipeState::Disconnected};
    AnimationSkipper m_animationSkipper;
    QList<spGameAction> m_ActionBuffer;
    QMutex m_ActionMutex;
    GameMap* m_pMap{nullptr};
    spGameMenue m_pMenu;
};

#endif // AIPROCESSPIPE_H
