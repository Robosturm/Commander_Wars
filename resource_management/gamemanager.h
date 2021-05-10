#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class GameManager : public QObject, public RessourceManagement<GameManager>
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

    QString getActionIcon(QString actionID);
    oxygine::spSprite getIcon(QString icon);
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
protected:
    friend RessourceManagement<GameManager>;
    GameManager();
private:
    virtual ~GameManager() = default;
    oxygine::spSprite getIconSprite(QString icon);
    QStringList m_loadedHeavyAis;
};

#endif // GAMEMANAGER_H
