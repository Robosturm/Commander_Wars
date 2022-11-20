#pragma once

#include <QObject>
#include "game/gameaction.h"
#include "game/gamemap.h"

class GameMenue;

class ActionPerformer final : public QObject
{
    Q_OBJECT
public:
    struct SyncData
    {
        bool m_waitingForSyncFinished{false};
        spGameAction m_postSyncAction;
        QVector<bool> m_lockedPlayers;
        QVector<quint64> m_connectingSockets;
    };

    explicit ActionPerformer(GameMap* pMap, GameMenue* pMenu = nullptr);
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

    bool getExit() const;
    void setExit(bool newExit);

signals:
    void sigActionPerformed();
    void sigAiProcesseSendAction(spGameAction pGameAction);

public slots:
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
    void performAction(spGameAction pGameAction);
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
    void delayedActionPerformed();
    /**
     * @brief getActionRunning
     * @return
     */
    bool getActionRunning() const;
    /**
     * @brief doTrapping
     * @param pGameAction
     */
    void doTrapping(spGameAction & pGameAction);    
    /**
     * @brief getSyncCounter
     * @return
     */
    qint64 getSyncCounter() const;
    /**
     * @brief nextTurnPlayerTimeout
     */
    void nextTurnPlayerTimeout();

protected:
    bool requiresForwarding(const spGameAction & pGameAction) const;

private:
    GameMenue* m_pMenu{nullptr};
    GameMap* m_pMap{nullptr};
    spGameAction m_pStoredAction{nullptr};
    spGameAction m_pCurrentAction{nullptr};
    bool m_actionRunning{false};
    bool m_noTimeOut{false};
    qint64 m_syncCounter{0};
    SyncData m_multiplayerSyncData;
    QTimer m_delayedActionPerformedTimer{this};
    bool m_exit{false};
};

