#ifndef DIALOGATTACKLOG_H
#define DIALOGATTACKLOG_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

class Player;
class GameMap;

class AttackReport;
using spAttackReport = std::shared_ptr<AttackReport>;

class DialogAttackLog;
using spDialogAttackLog = std::shared_ptr<DialogAttackLog>;

class DialogAttackLog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogAttackLog(GameMap* pMap, Player* pPlayer);
    virtual ~DialogAttackLog() = default;
signals:
    void sigFinished();
    void sigShowAttack(qint32 posAtkX, qint32 posAtkY, qint32 playerAtk, qint32 posDefX, qint32 posDefY, qint32 playerDef);
public slots:
    void showAttack(qint32 posAtkX, qint32 posAtkY, qint32 playerAtk, qint32 posDefX, qint32 posDefY, qint32 playerDef);
private slots:
    void remove();
private:
    QVector<spAttackReport> m_Log;
    Player* m_pPlayer{nullptr};
    GameMap* m_pMap{nullptr};
};

#endif // DIALOGATTACKLOG_H
