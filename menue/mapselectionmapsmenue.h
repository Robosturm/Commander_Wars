#ifndef MAPSELECTIONMAPSMENUE_H
#define MAPSELECTIONMAPSMENUE_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/mapselection.h"

#include "objects/minimap.h"

#include "objects/coselection.h"

#include "objects/panel.h"

class GameMap;

class MapSelectionMapsMenue : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    enum class MapSelectionStep
    {
        selectMap,
        selectRules,
        selectPlayer
    };

    explicit MapSelectionMapsMenue();
    virtual ~MapSelectionMapsMenue();

    void hideMapSelection();
    void showMapSelection();
    void hideCOSelection();
    void showCOSelection();
signals:
    void buttonBack();
    void buttonNext();
public slots:
    void mapSelectionItemClicked(QString item);
    void mapSelectionItemChanged(QString item);
    void slotButtonBack();
    void slotButtonNext();
private:
    GameMap* m_pCurrentMap{nullptr};
    // map selection
    spMapSelection m_pMapSelection;
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_MinimapSlider;
    oxygine::spBox9Sprite m_pMiniMapBox;
    QVector<oxygine::spTextField> m_BuildingCountTexts;
    oxygine::spBox9Sprite m_pBuildingBackground;
    // rule selection

    // player selection
    spCOSelection m_pCOSelection;
    spPanel m_pPlayerSelection;


    MapSelectionStep m_MapSelectionStep{MapSelectionStep::selectMap};
};

#endif // MAPSELECTIONMENUE_H
