#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

#include "QString"
#include "QStringList"

#include <QObject>

class Unit;

class WeaponManager : public QObject
{
    Q_OBJECT
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
    bool loadWeapon(QString weaponID);
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
    float getBaseDamage(QString weaponID, Unit* pDefender);
    /**
     * @brief getEnviromentDamage
     * @param weaponID
     * @param terrainID
     * @return
     */
    float getEnviromentDamage(QString weaponID, QString terrainID);
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
    /**
     * @brief existsWeapon
     * @param weaponID
     * @return
     */
    bool existsWeapon(QString weaponID);
private:
    explicit WeaponManager();
    virtual ~WeaponManager() = default;
    QStringList m_loadedWeapons;
    static WeaponManager* m_pInstance;
};

#endif // WEAPONMANAGER_H
