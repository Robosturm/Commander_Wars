#ifndef EDITORMENUE_H
#define EDITORMENUE_H

#include "menue/ingamemenue.h"

#include <QObject>

#include "objects/editorselection.h"

class EditorMenue : public InGameMenue
{
    Q_OBJECT
public:
    EditorMenue();

signals:
    void sigOnMapClickedLeft();
    void sigOnMapClickedRight();
public slots:
    /**
     * @brief cursorMoved called when the cursor moved
     */
    void cursorMoved();
    /**
     * @brief onMapClickedLeft left click on the map
     */
    void onMapClickedLeft();
    /**
     * @brief onMapClickedRight right click on the map
     */
    void onMapClickedRight();
    /**
     * @brief placeTerrain places  the terrain here
     * @param x
     * @param y
     */
    void placeTerrain(qint32 x, qint32 y);
    /**
     * @brief placeBuilding places the building here
     * @param x
     * @param y
     */
    void placeBuilding(qint32 x, qint32 y);
    /**
     * @brief placeUnit places the unit here
     * @param x
     * @param y
     */
    void placeUnit(qint32 x, qint32 y);
    /**
     * @brief canTerrainBePlaced checks if the terrain can be placed here
     * @param x
     * @param y
     * @return
     */
    bool canTerrainBePlaced(qint32 x, qint32 y);
    /**
     * @brief canBuildingBePlaced checks if the building can be placed here
     * @param x
     * @param y
     * @return
     */
    bool canBuildingBePlaced(qint32 x, qint32 y);
    /**
     * @brief canUnitBePlaced checks if the unit can be placed here
     * @param x
     * @param y
     * @return
     */
    bool canUnitBePlaced(qint32 x, qint32 y);

private:
    spEditorSelection m_EditorSelection{new EditorSelection()};
};

#endif // EDITORMENUE_H
