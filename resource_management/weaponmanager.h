#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

#include "QString"
#include "QStringList"

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

    qint32 getWeaponCount()
    {
        return m_loadedWeapons.size();
    }
private:
    explicit WeaponManager();
    virtual ~WeaponManager() = default;
    QStringList m_loadedWeapons;
    static WeaponManager* m_pInstance;
};

#endif // WEAPONMANAGER_H
