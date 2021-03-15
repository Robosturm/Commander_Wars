#ifndef DIALOGATTACKLOG_H
#define DIALOGATTACKLOG_H

#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Player;

class AttackReport;
typedef oxygine::intrusive_ptr<AttackReport> spAttackReport;

class DialogAttackLog;
typedef oxygine::intrusive_ptr<DialogAttackLog> spDialogAttackLog;

class DialogAttackLog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogAttackLog(Player* pPlayer);

signals:
    void sigFinished();
    void sigShowAttack(qint32 posAtkX, qint32 posAtkY, qint32 playerAtk, qint32 posDefX, qint32 posDefY, qint32 playerDef);
public slots:
    void showAttack(qint32 posAtkX, qint32 posAtkY, qint32 playerAtk, qint32 posDefX, qint32 posDefY, qint32 playerDef);
private:
    QVector<spAttackReport> m_Log;
    Player* m_pPlayer{nullptr};
};

#endif // DIALOGATTACKLOG_H
