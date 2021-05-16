#ifndef BUILDINGSPRITEMANAGER_H
#define BUILDINGSPRITEMANAGER_H

#include <qobject.h>
#include "resource_management/ressourcemanagement.h"

class BuildingSpriteManager : public QObject, public RessourceManagement<BuildingSpriteManager>
{
    Q_OBJECT
public slots:
    void removeRessource(QString id);
protected:
    friend RessourceManagement<BuildingSpriteManager>;
    BuildingSpriteManager();
private:
    virtual ~BuildingSpriteManager() = default;
};

#endif // BUILDINGSPRITEMANAGER_H
