#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

class Unit;

#include "resource_management/RessourceManagement.h"

class WeaponManager : public QObject, public RessourceManagement<WeaponManager>
{
    Q_OBJECT
public:
    /**
     * @brief getBaseDamage
     * @param weaponID
     * @param pDefender
     * @return
     */
    float getBaseDamage(QString weaponID, Unit* pDefender);
    /**
     * @brief getEnviromentDamage
     * @param weaponID
     * @param terrainID
     * @return
     */
    float getEnviromentDamage(QString weaponID, QString terrainID);
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
        setObjectName("WeaponManager");
    }
private:
    virtual ~WeaponManager() = default;
};

#endif // WEAPONMANAGER_H
