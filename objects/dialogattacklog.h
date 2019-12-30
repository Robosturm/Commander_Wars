#ifndef DIALOGATTACKLOG_H
#define DIALOGATTACKLOG_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

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
private:
    QVector<spAttackReport> m_Log;
};

#endif // DIALOGATTACKLOG_H
