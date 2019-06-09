#ifndef BATTLEANIMATIONMANAGER_H
#define BATTLEANIMATIONMANAGER_H

#include "QString"
#include "QStringList"
#include "oxygine-framework.h"

class BattleAnimationManager : public oxygine::Resources
{
public:

    /**
     * @brief getInstance this object
     * @return
     */
    static BattleAnimationManager* getInstance();
    /**
     * @brief loadAll loads all units data
     */
    void loadAll();
    /**
     * @brief loadBattleAnimations
     * @param battleAnimationsID the battleAnimations that should be loaded.
     * @return true if all is fine
     */
    bool loadBattleAnimations(const QString& battleAnimationsID);
    /**
     * @brief getUnitID
     * @param position the position in the loaded unit list
     * @return the id of the unit at the given index
     */
    inline QString getBattleAnimationsID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedBattleAnimations.size()))
        {
            return m_loadedBattleAnimations.at(position);
        }
        return "";
    }
    /**
     * @brief getBattleAnimationsIndex
     * @param id
     * @return
     */
    qint32 getBattleAnimationsIndex(QString id);
    /**
     * @brief getBattleAnimationsCount
     * @return the amount of battle animations loaded at the current time
     */
    inline qint32 getBattleAnimationsCount()
    {
        return m_loadedBattleAnimations.size();
    }
    /**
     * @brief reset deletes all data
     */
    void reset();
    /**
     * @brief existsBattleAnimations
     * @param battleAnimationsID
     */
    bool existsBattleAnimations(QString battleAnimationsID);
    /**
      * @brief getLoadedBattleAnimations
      * @return
      */
    inline QStringList getLoadedBattleAnimations()
    {
        return m_loadedBattleAnimations;
    }
private:
    BattleAnimationManager();
    virtual ~BattleAnimationManager() = default;
    QStringList m_loadedBattleAnimations;
    static BattleAnimationManager* m_pInstance;
};

#endif // BATTLEANIMATIONMANAGER_H
