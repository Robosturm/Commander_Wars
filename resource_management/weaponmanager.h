#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

#include "QString"

class WeaponManager
{
public:
    /**
     * @brief getInstance this object
     * @return
     */
    static WeaponManager* getInstance();
    /**
     * @brief loadAll loads all weapons data
     */
    void loadAll();
    /**
     * @brief loadWeapon
     * @param weaponID
     */
    bool loadWeapon(const QString& weaponID);
    /**
     * @brief reset deletes all data
     */
    void reset();
private:
    explicit WeaponManager();
    virtual ~WeaponManager() = default;
    QStringList m_loadedWeapons;
    static WeaponManager* m_pInstance;
};

#endif // WEAPONMANAGER_H
