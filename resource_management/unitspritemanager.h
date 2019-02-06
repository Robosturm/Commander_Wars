#ifndef UNITSPRITEMANAGER_H
#define UNITSPRITEMANAGER_H

#include "QString"
#include "oxygine-framework.h"

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
    }
    /**
     * @brief getUnitCount
     * @return the amount of units loaded at the current time
     */
    inline qint32 getUnitCount()
    {
        return m_loadedUnits.size();
    }
    /**
     * @brief reset deletes all data
     */
    void reset();
    /**
     * @brief existsUnit
     * @param unitID
     */
    bool existsUnit(QString unitID);
private:
    explicit UnitSpriteManager();
    virtual ~UnitSpriteManager() = default;
    QStringList m_loadedUnits;
    static UnitSpriteManager* m_pInstance;
};

#endif // UNITSPRITEMANAGER_H
