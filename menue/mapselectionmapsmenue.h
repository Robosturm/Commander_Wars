#ifndef MAPSELECTIONMAPSMENUE_H
#define MAPSELECTIONMAPSMENUE_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/mapselection.h"

#include "objects/minimap.h"

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
signals:
    void buttonBack();
    void buttonNext();
public slots:
    void mapSelectionItemClicked(QString item);
    void mapSelectionItemChanged(QString item);
    void slotButtonBack();
    void slotButtonNext();
private:
    spMapSelection m_pMapSelection;
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_MinimapSlider;
    oxygine::spBox9Sprite m_pMiniMapBox;
    QVector<oxygine::spTextField> m_BuildingCountTexts;
    oxygine::spBox9Sprite m_pBuildingBackground;
    GameMap* m_pCurrentMap{nullptr};

    MapSelectionStep m_MapSelectionStep{MapSelectionStep::selectMap};
};

#endif // MAPSELECTIONMENUE_H
