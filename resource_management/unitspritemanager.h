#ifndef UNITSPRITEMANAGER_H
#define UNITSPRITEMANAGER_H

#include "game/unit.h"

#include "resource_management/ressourcemanagement.h"

class UnitSpriteManager final : public QObject, public RessourceManagement<UnitSpriteManager>
{
    Q_OBJECT
public:
    /**
     * @brief createBaseDamageTable
     */
    void createBaseDamageTable();

    qint32 getBaseDamage(Unit* pAttacker, Unit* pDefender)
    {
        return getBaseDamage(pAttacker->getUnitIdx(), pDefender->getUnitIdx());
    }
    qint32 getBaseDamage(qint32 attackerIdx, qint32 defenderIdx)
    {
        return m_baseDamgeTable[attackerIdx * m_loadedRessources.size() + defenderIdx];
    }
    /**
     * @brief getUnitType
     * @param i
     * @return
     */
    qint32 getUnitType(qint32 i);
    /**
     * @brief UnitSpriteManager::getUnitType
     * @param id
     * @return
     */
    qint32 getUnitType(const QString & id);
    /**
     * @brief getMovementType
     * @param id
     * @return
     */
    QString getMovementType(const QString & id);
    /**
     * @brief getUnitTypeText
     * @param type
     * @return
     */
    static QString getUnitTypeText(qint32 type);
public slots:
    void removeRessource(QString id);
    /**
     * @brief getUnitsSorted
     * @return
     */
    QStringList getUnitsSorted();
    /**
     * @brief getCount
     * @return
     */
    qint32 getUnitCount()
    {
        return m_loadedRessources.size();
    }
protected:
    friend MemoryManagement;
    UnitSpriteManager();
private:
    virtual ~UnitSpriteManager() = default;

    std::map<qint32, float> m_baseDamgeTable;
};

Q_DECLARE_INTERFACE(UnitSpriteManager, "UnitSpriteManager");

#endif // UNITSPRITEMANAGER_H
