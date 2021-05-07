#ifndef DIALOGUNITINFO_H
#define DIALOGUNITINFO_H

#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Player;

class DialogUnitInfo;
typedef oxygine::intrusive_ptr<DialogUnitInfo> spDialogUnitInfo;

class DialogUnitInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogUnitInfo(Player* pPlayer);

signals:
    void sigFinished();
    void sigMoveToUnit(qint32 posX, qint32 posY);
public slots:
    void moveToUnit(qint32 posX, qint32 posY);
};

#endif // DIALOGUNITINFO_H
