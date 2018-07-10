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
    void sigOnMapClicked();
public slots:
    void cursorMoved();
    void onMapClicked();
    void placeTerrain(qint32 x, qint32 y);
    bool canTerrainBePlaced(qint32 x, qint32 y);
private:
    spEditorSelection m_EditorSelection{new EditorSelection()};
};

#endif // EDITORMENUE_H
