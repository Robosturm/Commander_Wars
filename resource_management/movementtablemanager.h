#ifndef MOVEMENTTABLEMANAGER_H
#define MOVEMENTTABLEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class Unit;
class Terrain;

class MovementTableManager final : public QObject, public RessourceManagement<MovementTableManager>
{
    Q_OBJECT
public:
    /**
     * @brief loadAll
     */
    virtual void loadAll() override;
public slots:
    qint32 getBaseMovementPoints(const QString & movementID, Terrain* pTerrain, Terrain* pCurrentTerrain, Unit* pUnit, bool trapChecking = false);
    bool getSupportsFastPfs(const QString & movementID);
protected:
    friend RessourceManagement<MovementTableManager>;
    MovementTableManager();
private:
    ~MovementTableManager() = default;
};

#endif // MOVEMENTTABLEMANAGER_H
