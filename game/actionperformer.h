#pragma once

#include <QObject>
#include "game/gameaction.h"
#include "game/gamemap.h"

class GameMenue;
using spGameMenue = std::shared_ptr<GameMenue>;
using spWeakGameMenue = std::weak_ptr<GameMenue>;

class ActionPerformer final : public QObject
{
    Q_OBJECT
public:
    struct ConnectingInfo
    {
        ConnectingInfo(quint64 socketId)
            : m_socketId(socketId)
        {
        }
        quint64 m_socketId;
        bool m_syncSend{false};
    };

    struct SyncData
    {
        bool m_waitingForSyncFinished{false};
        spGameAction m_postSyncAction{nullptr};
        QVector<bool> m_lockedPlayers;
        QVector<ConnectingInfo> m_connectingSocketInfos;
    };

    explicit ActionPerformer(spWeakGameMap pMap, oxygine::spWeakEventDispatcher pMenu);
    /**
     * @brief getSyncData
     * @return
     */
    SyncData & getSyncData()
    {
        return m_multiplayerSyncData;
    }
    /**
     * @brief setSyncCounter
     * @param counter
     */
    void setSyncCounter(qint64 counter);
    /**
     * @brief setActionRunning
     * @param newActionRunning
     */
    void setActionRunning(bool newActionRunning);
    Q_INVOKABLE bool getExit() const;
    void setExit(bool newExit);
    /**
     * @brief centerMapOnAction
     * @param pGameAction
     */
    void centerMapOnAction(GameAction* pGameAction);
    /**
     * @brief actionPerformed
     */
    void actionPerformed();
    /**
     * @brief performAction performs the given action and deletes it afterwards.
     * @param pGameAction
     */
    Q_INVOKABLE void performAction(spGameAction pGameAction, bool fromAiPipe = false);
    /**
     * @brief isTrap
     * @param pAction
     * @param pMoveUnit
     * @param currentPoint
     * @param previousPoint
     * @return
     */
    bool isTrap(const QString & function, spGameAction pAction, Unit* pMoveUnit, QPoint currentPoint, QPoint previousPoint, qint32 moveCost);
    /**
     * @brief finsihActionPerformed
     */
    void finishActionPerformed();
    /**
     * @brief skipAnimations
     */
    void skipAnimations(bool postAnimation);
    /**
     * @brief doMultiTurnMovement
     * @param pGameAction
     */
    spGameAction doMultiTurnMovement(spGameAction pGameAction);
    /**
     * @brief delayedActionPerformed
     */
    Q_INVOKABLE void delayedActionPerformed();
    /**
     * @brief getActionRunning
     * @return
     */
    Q_INVOKABLE bool getActionRunning() const;
    /**
     * @brief getSyncCounter
     * @return
     */
    Q_INVOKABLE qint64 getSyncCounter() const;
    /**
     * @brief nextTurnPlayerTimeout
     */
    Q_INVOKABLE void nextTurnPlayerTimeout();
signals:
    void sigPerformAction(spGameAction pGameAction, bool fromAiPipe = false);
    void sigActionPerformed();
    void sigAiProcesseSendAction(spGameAction pGameAction);

protected:
    bool requiresForwarding(const spGameAction & pGameAction) const;
    /**
     * @brief doTrapping
     * @param pGameAction
     */
    void doTrapping(spGameAction & pGameAction);
    void onTriggeringActionFinished();
private:
    oxygine::spWeakEventDispatcher m_pMenu;
    spWeakGameMap m_pMap;
    spGameAction m_pStoredAction{nullptr};
    spGameAction m_pCurrentAction{nullptr};
    bool m_actionRunning{false};
    bool m_noTimeOut{false};
    bool m_finishedPerformed{false};
    qint64 m_syncCounter{0};
    SyncData m_multiplayerSyncData;
    QTimer m_delayedActionPerformedTimer{this};
    bool m_exit{false};
    QByteArray m_mapHash;
};

