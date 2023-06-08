#ifndef UNITSPRITEMANAGER_H
#define UNITSPRITEMANAGER_H

#include "game/GameEnums.h"

#include "resource_management/ressourcemanagement.h"

class UnitSpriteManager final : public QObject, public RessourceManagement<UnitSpriteManager>
{
    Q_OBJECT
public:
    /**
     * @brief getUnitType
     * @param i
     * @return
     */
    qint32 getUnitType(qint32 i);
    /**
     * @brief UnitSpriteManager::getUnitType
     * @param id
     * @return
     */
    qint32 getUnitType(const QString & id);
    /**
     * @brief getMovementType
     * @param id
     * @return
     */
    QString getMovementType(const QString & id);
    /**
     * @brief getUnitTypeText
     * @param type
     * @return
     */
    static QString getUnitTypeText(qint32 type);
public slots:
    void removeRessource(QString id);
    /**
     * @brief getUnitsSorted
     * @return
     */
    QStringList getUnitsSorted();
    /**
     * @brief getCount
     * @return
     */
    qint32 getUnitCount()
    {
        return m_loadedRessources.size();
    }
protected:
    friend RessourceManagement<UnitSpriteManager>;
    UnitSpriteManager();
private:
   virtual ~UnitSpriteManager() = default;
};

Q_DECLARE_INTERFACE(UnitSpriteManager, "UnitSpriteManager");

#endif // UNITSPRITEMANAGER_H
