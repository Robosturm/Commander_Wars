#pragma once

#include <QObject>
#include <QPoint>

#include "game/gamemap.h"

#include "objects/base/dropdownmenusprite.h"
#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"
#include "objects/base/label.h"
#include "objects/base/closeablepopup.h"

class DamageCalculator;
using spDamageCalculator = std::shared_ptr<DamageCalculator>;

class DamageCalculator final : public CloseablePopUp
{
    Q_OBJECT
    struct CoData
    {
        spDropDownmenuSprite m_co;
        spCheckbox m_inCoRange;
        spDropDownmenuSprite m_powerMode;
    };
    struct UnitData
    {
        spDropDownmenuSprite m_unit;
        spDropDownmenuSprite m_Terrain;
        spDropDownmenuSprite m_unitRank;
        spSpinBox m_hp;
        spSpinBox m_towers;
        spSpinBox m_funds;
        spSpinBox m_buildings;
        spLabel m_minBattleDamage;
        spLabel m_avgBattleDamage;
        spLabel m_maxBattleDamage;
        spLabel m_minPostbattleDamage;
        spLabel m_avgPostbattleDamage;
        spLabel m_maxPostbattleDamage;
    };
    using CosData = CoData[2];
public:
    explicit DamageCalculator(GameMap * pMap);
    virtual ~DamageCalculator() = default;

signals:
    void sigCalculateDamage();
private slots:
    void calculateDamage();
private:
    void updateMapData(QPoint & defPos, bool forceDirect = false);
    void loadCoData(qint32 & x, qint32 & y, CosData & cosData, const QStringList & coIds);
    void loadUnitData(qint32 & x, qint32 & y, UnitData & unitData,
                      const QStringList & rankItems, const QStringList & unitIds,
                      const QStringList & terrainIds, const QStringList & buildingIds);
    void placeBuilding(QString & buildingId, qint32 player, qint32 count, qint32 x, QString unitBuilding);
    void createDamageText(spLabel & minText, spLabel & avgText, spLabel & maxText,
                          qint32 minDmg, qint32 avgDmg, qint32 maxDmg);
    QRectF calculatePostBattleDamage(spUnit pAttacker, spUnit pDefender, QRectF & info);
private:
    GameMap m_map;
    UnitData m_atkUnit;
    CosData m_atkCos;
    UnitData m_defUnit;
    CosData m_defCos;
    spPlayer m_dropDownPlayer;
};

