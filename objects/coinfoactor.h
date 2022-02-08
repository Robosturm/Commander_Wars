#ifndef COINFOACTOR_H
#define COINFOACTOR_H

#include "qobject.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include <QVector>

#include "game/player.h"
#include "game/co.h"
#include "game/ui/copowermeter.h"
#include "game/unit.h"

class COInfoActor;
using spCOInfoActor = oxygine::intrusive_ptr<COInfoActor>;

class COInfoActor : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COInfoActor(GameMap* pMap, qint32 width);
    virtual ~COInfoActor() = default;
    /**
     * @brief showCO
     * @param pCO
     */
    void showCO(spCO pCO, spPlayer pPlayer);
    /**
     * @brief createStrengthBar
     * @param pActor
     * @param bonus
     * @param y
     */
    void createStrengthBar(oxygine::spActor pActor, qint32 bonus, qint32 y);
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
protected:
    /**
     * @brief showPerks
     * @param pCO
     * @param y
     */
    void showPerks(spCO pCO, qint32 & y);
    /**
     * @brief showCOBoost
     * @param pUnit
     * @param pCO
     * @param x
     * @param y
     */
    void showCOBoost(spUnit pUnit, spCO pCO, qint32 & x, qint32 & y);
    /**
     * @brief showCustomCOBoost
     * @param pCO
     * @param x
     * @param y
     * @param index
     * @param type
     */
    void showCustomCOBoost(spCO pCO, qint32 & x, qint32 & y, qint32 index, bool global);
protected:
    spCoPowermeter m_pCoPowermeter{nullptr};

    oxygine::spSprite m_pCurrentCoFaction;
    oxygine::spSprite m_pCurrentCO;

    oxygine::spTextField m_COName;
    oxygine::spTextField m_COBio;

    oxygine::spBox9Sprite m_HitSprite;
    oxygine::spTextField m_HitText;

    oxygine::spSprite m_MissSprite;
    oxygine::spTextField m_MissText;

    oxygine::spSprite m_InfoSprite;
    oxygine::spTextField m_InfoText;

    oxygine::spTextField m_powerProgress;

    oxygine::spSprite m_PowerSprite;
    oxygine::spTextField m_Powername;
    oxygine::spTextField m_PowerDesc;

    oxygine::spSprite m_SuperPowerSprite;
    oxygine::spTextField m_SuperPowername;
    oxygine::spTextField m_SuperPowerDesc;

    oxygine::spTextField m_GlobalBoosts;
    oxygine::spTextField m_CoBoost;

    oxygine::spTextField m_SynergyText;
    QVector<oxygine::spTextField> m_SynergyCONames;
    QVector<oxygine::spActor> m_SynergyStarActors;

    oxygine::spTextField m_PerkText;
    QVector<oxygine::spActor> m_PerkActors;

    QVector<oxygine::spActor> m_UnitDataActors;
    GameMap* m_pMap{nullptr};
};

#endif // COINFOACTOR_H
