#ifndef COPERKMANAGER_H
#define COPERKMANAGER_H

#include "resource_management/ressourcemanagement.h"

class COPerkManager : public QObject, public RessourceManagement<COPerkManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<COPerkManager>;
    COPerkManager();
public:
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
     * @brief getDescription
     * @param position
     * @return
     */
    QString getDescription(qint32 position);
private:
    virtual ~COPerkManager() = default;
};

#endif // COPERKMANAGER_H
