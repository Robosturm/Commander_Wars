#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>

#include "oxygine-framework.h"
#include "game/smartpointers.h"


class Building : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Building(const QString& BuildingID, qint32 x, qint32 y);

signals:

public slots:

private:
    oxygine::spSprite m_pBuildingSprite{nullptr};
    /**
     * @brief m_BuildingID the id of this terrain
     */
    QString m_BuildingID;
    /**
     * @brief m_Owner our owner a nullptr means we're a neutral building
     */
    spPlayer m_Owner{nullptr};
    /**
     * @brief m_Terrain the terrain at which we are placed
     */
    spTerrain m_Terrain{nullptr};
};

#endif // BUILDING_H
