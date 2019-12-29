#ifndef DIALOGATTACKLOG_H
#define DIALOGATTACKLOG_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

class Player;

class AttackReport;
typedef oxygine::intrusive_ptr<AttackReport> spAttackReport;

class DialogAttackLog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogAttackLog(Player* pPlayer);

signals:

private:
    QVector<spAttackReport> m_Log;
};

#endif // DIALOGATTACKLOG_H
