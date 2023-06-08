#ifndef DIALOGUNITINFO_H
#define DIALOGUNITINFO_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

class Player;

class DialogUnitInfo;
using spDialogUnitInfo = oxygine::intrusive_ptr<DialogUnitInfo>;

class DialogUnitInfo final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogUnitInfo(Player* pPlayer);
   virtual ~DialogUnitInfo() = default;
signals:
    void sigFinished();
    void sigMoveToUnit(qint32 posX, qint32 posY);
public slots:
    void moveToUnit(qint32 posX, qint32 posY);
private slots:
    void remove();
private:
    Player* m_pPlayer{nullptr};
};

#endif // DIALOGUNITINFO_H
