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
    void cursorMoved();
    void onMapClickedLeft();
    void onMapClickedRight();
    void placeTerrain(qint32 x, qint32 y);
    void placeBuilding(qint32 x, qint32 y);
    bool canTerrainBePlaced(qint32 x, qint32 y);
    bool canBuildingBePlaced(qint32 x, qint32 y);

private:
    spEditorSelection m_EditorSelection{new EditorSelection()};
};

#endif // EDITORMENUE_H
