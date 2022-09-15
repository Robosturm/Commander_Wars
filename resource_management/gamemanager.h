#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class GameMap;

class GameManager final : public QObject, public RessourceManagement<GameManager>
{
    Q_OBJECT
public:
    /**
     * @brief loadAll
     */
    virtual void loadAll() override;
    /**
     * @brief reset
     */
    virtual void reset() override;

    QString getActionIcon(const QString & actionID);
    oxygine::spSprite getIcon(GameMap* pMap, const QString & icon);
    /**
     * @brief getDescription
     * @param position
     * @return
     */
    QString getDescription(qint32 position);
    /**
     * @brief getVictoryRuleID
     * @param position the position in the loaded victory list
     * @return the id of the victory rule at the given index
     */
    inline QString getHeavyAiID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedHeavyAis.size()))
        {
            return m_loadedHeavyAis.at(position);
        }
        return "";
    }
    /**
     * @brief getVictoryRuleCount
     * @return the amount of victory conditions loaded at the current time
     */
    inline qint32 getHeavyAiCount()
    {
        return m_loadedHeavyAis.size();
    }
    /**
     * @brief getDefaultActionbBannlist
     * @return
     */
    QStringList getDefaultActionbBannlist();
protected:
    friend RessourceManagement<GameManager>;
    GameManager();
private:
    ~GameManager() = default;
    oxygine::spSprite getIconSprite(const QString & icon);
    QStringList m_loadedHeavyAis;
    QStringList m_loadedNormalAis;
    QStringList m_loadedVeryEasyAis;
};

#endif // GAMEMANAGER_H
