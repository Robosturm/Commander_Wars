#ifndef UNITINFO_H
#define UNITINFO_H

#include <QObject>

#include "oxygine-framework.h"

#include "game/unit.h"

class UnitInfo;
typedef oxygine::intrusive_ptr<UnitInfo> spUnitInfo;

class UnitInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit UnitInfo(Unit* pUnit, qint32 width);
    /**
     * @brief createWeaponTable
     * @param weaponID
     */
    void createWeaponTable(Unit* pUnit, QString weaponID, qint32& y, qint32 width);
    /**
     * @brief createLoadingTable
     * @param pUnit
     * @param loadables
     * @param y
     * @param width
     */
    void createLoadingTable(Unit* pUnit, QStringList loadables, qint32& y, qint32 width);
};

#endif // UNITINFO_H
