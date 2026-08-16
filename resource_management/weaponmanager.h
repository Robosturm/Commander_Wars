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
    Q_INVOKABLE float getBaseDamage(const QString & weaponID, Unit* pDefender);
    /**
     * @brief getEnviromentDamage
     * @param weaponID
     * @param terrainID
     * @return
     */
    Q_INVOKABLE float getEnviromentDamage(const QString & weaponID, const QString & terrainID);
    /**
     * @brief getWeaponName
     * @param id
     * @return
     */
    Q_INVOKABLE QString getWeaponName(const QString & id);
    /**
     * @brief loadAll loads all weapons data
     */
    virtual void loadAll() override;
protected:
    friend MemoryManagement;
    WeaponManager()
        : RessourceManagement<WeaponManager>("",
                                             "/scripts/weapons")
    {
        Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
        setObjectName("WeaponManager");
#endif
    }
private:
    virtual ~WeaponManager() = default;
};

#endif // WEAPONMANAGER_H
