#ifndef COPERKMANAGER_H
#define COPERKMANAGER_H

#include "resource_management/ressourcemanagement.h"

class COPerkManager final : public QObject, public RessourceManagement<COPerkManager>
{
    Q_OBJECT
public slots:
    /**
     * @brief getLoadedPerks
     * @return
     */
    QStringList getLoadedPerks()
    {
        return m_loadedRessources;
    }
    /**
     * @brief getIcon
     * @param index
     * @return
     */
    QString getIcon(qint32 position);
    /**
     * @brief isSelectable
     * @param position
     * @return
     */
    bool isSelectable(qint32 position);
    /**
     * @brief isSelectable
     * @param id
     * @return
     */
    bool isSelectable(QString id);
    /**
     * @brief getDescription
     * @param position
     * @return
     */
    QString getDescription(qint32 position);
    /**
     * @brief getGroup
     * @param position
     * @return
     */
    QString getGroup(qint32 position);
    /**
     * @brief removeRessource
     * @param id
     */
    void removeRessource(QString id);
protected:
    friend MemoryManagement;
    COPerkManager();
    virtual ~COPerkManager() = default;
};

Q_DECLARE_INTERFACE(COPerkManager, "COPerkManager");

#endif // COPERKMANAGER_H
