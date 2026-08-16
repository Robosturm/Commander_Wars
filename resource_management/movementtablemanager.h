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
    virtual void loadAll() override;    /**
     * @brief getMovementName
     * @param id
     * @return
     */
    Q_INVOKABLE QString getMovementName(const QString & id);

    Q_INVOKABLE QStringList getMovementTypes();
public slots:
    qint32 getBaseMovementPoints(const QString & movementID, Terrain* pTerrain, Terrain* pCurrentTerrain, Unit* pUnit, bool trapChecking = false);
    bool getSupportsFastPfs(const QString & movementID);
protected:
    friend MemoryManagement;
    MovementTableManager();
private:
    virtual ~MovementTableManager() = default;
};

#endif // MOVEMENTTABLEMANAGER_H
