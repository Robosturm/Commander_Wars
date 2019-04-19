#ifndef BUILDINGSPRITEMANAGER_H
#define BUILDINGSPRITEMANAGER_H

#include "oxygine-framework.h"

#include <QObject>

class BuildingSpriteManager : public QObject, public oxygine::Resources
{
    Q_OBJECT
public:
    static BuildingSpriteManager* getInstance();

signals:

public slots:
    void loadAll();
    /**
     * @brief loadBuilding
     * @param buildingID the building that should be loaded.
     * @return true if all is fine
     */
    bool loadBuilding(const QString& buildingID);
    /**
     * @brief getBuildingID
     * @param position the position in the loaded building list
     * @return the id of the building at the given index
     */
    inline QString getBuildingID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedBuildings.size()))
        {
            return m_loadedBuildings.at(position);
        }
    }
    /**
     * @brief getBuildingIndex
     * @param id
     * @return
     */
    qint32 getBuildingIndex(QString id);
    /**
     * @brief getBuildingCount
     * @return the amount of buildings loaded at the current time
     */
    inline qint32 getBuildingCount()
    {
        return m_loadedBuildings.size();
    }
    void reset();
private:
    QStringList m_loadedBuildings;
    explicit BuildingSpriteManager();
    virtual ~BuildingSpriteManager() = default;
    static BuildingSpriteManager* m_pInstance;
};

#endif // BUILDINGSPRITEMANAGER_H
