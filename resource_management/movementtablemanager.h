#ifndef MOVEMENTTABLEMANAGER_H
#define MOVEMENTTABLEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class Unit;
class Terrain;

class MovementTableManager : public QObject, public RessourceManagement<MovementTableManager>
{
    Q_OBJECT
public:
    /**
     * @brief loadAll
     */
    virtual void loadAll() override;
public slots:
    qint32 getBaseMovementPoints(QString movementID, Terrain* pTerrain, Terrain* pCurrentTerrain, Unit* pUnit, bool trapChecking = false);
protected:
    friend RessourceManagement<MovementTableManager>;
    MovementTableManager();
private:
    virtual ~MovementTableManager() = default;
};

#endif // MOVEMENTTABLEMANAGER_H
