#ifndef UNITSPRITEMANAGER_H
#define UNITSPRITEMANAGER_H

#include "QString"
#include "QStringList"
#include "oxygine-framework.h"
#include "game/GameEnums.h"

class UnitSpriteManager : public oxygine::Resources
{
public:
    /**
     * @brief getInstance this object
     * @return
     */
    static UnitSpriteManager* getInstance();
    /**
     * @brief loadAll loads all units data
     */
    void loadAll();
    /**
     * @brief loadUnit
     * @param unitID the unit that should be loaded.
     * @return true if all is fine
     */
    bool loadUnit(const QString& unitID);
    /**
     * @brief getUnitID
     * @param position the position in the loaded unit list
     * @return the id of the unit at the given index
     */
    inline QString getUnitID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedUnits.size()))
        {
            return m_loadedUnits.at(position);
        }
        return "";
    }
    /**
     * @brief getUnitIndex
     * @param id
     * @return
     */
    qint32 getUnitIndex(QString id);
    /**
     * @brief getUnitCount
     * @return the amount of units loaded at the current time
     */
    inline qint32 getUnitCount()
    {
        return m_loadedUnits.size();
    }
    /**
     * @brief getUnitType
     * @param i
     * @return
     */
    GameEnums::UnitType getUnitType(qint32 i);
    /**
     * @brief reset deletes all data
     */
    void reset();
    /**
     * @brief existsUnit
     * @param unitID
     */
    bool existsUnit(QString unitID);
    /**
      * @brief getLoadedUnits
      * @return
      */
    inline QStringList getLoadedUnits()
    {
        return m_loadedUnits;
    }
    /**
     * @brief getUnitName
     * @param position
     * @return
     */
    QString getUnitName(qint32 position);
private:
    explicit UnitSpriteManager();
    virtual ~UnitSpriteManager() = default;
    QStringList m_loadedUnits;
    static UnitSpriteManager* m_pInstance;
};

#endif // UNITSPRITEMANAGER_H
