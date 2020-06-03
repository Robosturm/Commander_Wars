#ifndef UNITSPRITEMANAGER_H
#define UNITSPRITEMANAGER_H


#include "game/GameEnums.h"
#include "resource_management/RessourceManagement.h"

class UnitSpriteManager : public QObject, public RessourceManagement<UnitSpriteManager>
{
    Q_OBJECT
public:
    /**
     * @brief getUnitType
     * @param i
     * @return
     */
    GameEnums::UnitType getUnitType(qint32 i);
    /**
     * @brief UnitSpriteManager::getUnitType
     * @param id
     * @return
     */
    GameEnums::UnitType getUnitType(QString id);
    /**
     * @brief getUnitsSorted
     * @return
     */
    QStringList getUnitsSorted();
protected:
    friend RessourceManagement<UnitSpriteManager>;
    UnitSpriteManager();
private:
    virtual ~UnitSpriteManager() = default;
};


#endif // UNITSPRITEMANAGER_H
