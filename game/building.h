#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"
#include "game/smartpointers.h"

class Building : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Building(const QString& BuildingID, qint32 x, qint32 y);

    void setOwner(spPlayer pOwner);
    void updateBuildingSprites();
signals:

public slots:
    void loadSprite(QString sprite, bool addPlayerColor);
    inline qint32 getOwnerID()
    {
        if (m_Owner.get() != nullptr)
        {
            // todo get player id
            //m_Owner.
        }
        return -1;
    }
private:
    QVector<oxygine::spSprite> m_pBuildingSprites;
    /**
     * @brief m_BuildingID the id of this terrain
     */
    QString m_BuildingID;
    /**
     * @brief m_Owner our owner a nullptr means we're a neutral building
     */
    spPlayer m_Owner;
    /**
     * @brief m_Terrain the terrain at which we are placed
     */
    spTerrain m_Terrain;
};

#endif // BUILDING_H
