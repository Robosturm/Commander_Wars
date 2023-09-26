#ifndef UNITINFO_H
#define UNITINFO_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

#include "game/unit.h"
#include "game/gameanimation/battleanimationsprite.h"

class UnitInfo;
using spUnitInfo = std::shared_ptr<UnitInfo>;

class UnitInfo final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit UnitInfo(spUnit pUnit, qint32 width);
    virtual ~UnitInfo() = default;
    /**
     * @brief createWeaponTable
     * @param weaponID
     */
    void createWeaponTable(Unit* pUnit, const QString & weaponID, qint32& y, qint32 width);
    /**
     * @brief createLoadingTable
     * @param pUnit
     * @param loadables
     * @param y
     * @param width
     */
    void createLoadingTable(Unit* pUnit, const QStringList & loadables, qint32& y, qint32 width);
    /**
     * @brief createLoadedUnits
     * @param pUnit
     * @param y
     * @param width
     */
    void createLoadedUnits(Unit* pUnit, qint32& y, qint32 width);
    /**
     * @brief createTransportTable
     * @param pUnit
     * @param y
     * @param width
     */
    void createTransportTable(Unit* pUnit, qint32& y, qint32 width);
    /**
     * @brief createActionTable
     * @param pUnit
     * @param y
     * @param width
     */
    void createActionTable(Unit* pUnit, qint32& y, qint32 width);

signals:
    /**
     * @brief sigShowLink
     * @param pageID
     */
    void sigShowLink(QString pageID);

public slots:
    /**
     * @brief showLink
     * @param pageID
     */
    void showLink(QString pageID);
    void showNewBattlesprite(QString newArmy);

private:
    void loadBattleSprite(Unit* pUnit);
    void loadArmySelector(qint32 x, qint32 y);

private:
    spBattleAnimationSprite m_pBattleAnimationSprite;
    oxygine::spBox9Sprite m_pSpriteBox;
    spUnit m_pUnit;
};

#endif // UNITINFO_H
