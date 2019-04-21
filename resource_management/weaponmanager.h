#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

#include "QString"
#include "QStringList"

class Unit;

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
    /**
     * @brief getBaseDamage
     * @param weaponID
     * @param pDefender
     * @return
     */
    float getBaseDamage(const QString& weaponID, Unit* pDefender);
    /**
     * @brief getWeaponCount
     * @return
     */
    qint32 getWeaponCount()
    {
        return m_loadedWeapons.size();
    }
    /**
     * @brief getWeaponName
     * @param weaponID
     * @return
     */
    QString getWeaponName(QString weaponID);
private:
    explicit WeaponManager();
    virtual ~WeaponManager() = default;
    QStringList m_loadedWeapons;
    static WeaponManager* m_pInstance;
};

#endif // WEAPONMANAGER_H
