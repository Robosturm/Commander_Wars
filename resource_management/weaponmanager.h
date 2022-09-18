#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

class Unit;

#include "resource_management/ressourcemanagement.h"

class WeaponManager final : public QObject, public RessourceManagement<WeaponManager>
{
    Q_OBJECT
public:
    /**
     * @brief getBaseDamage
     * @param weaponID
     * @param pDefender
     * @return
     */
    float getBaseDamage(const QString & weaponID, Unit* pDefender);
    /**
     * @brief getEnviromentDamage
     * @param weaponID
     * @param terrainID
     * @return
     */
    float getEnviromentDamage(const QString & weaponID, const QString & terrainID);
    /**
     * @brief loadAll loads all weapons data
     */
    virtual void loadAll() override;
protected:
    friend RessourceManagement<WeaponManager>;
    WeaponManager()
        : RessourceManagement<WeaponManager>("",
                                             "/scripts/weapons")
    {
#ifdef GRAPHICSUPPORT
        setObjectName("WeaponManager");
#endif
    }
private:
    ~WeaponManager() = default;
};

#endif // WEAPONMANAGER_H
