#ifndef MAPSELECTIONMAPSMENUE_H
#define MAPSELECTIONMAPSMENUE_H

#include <QObject>

#include "QFileInfo"

#include "oxygine-framework.h"

#include "objects/mapselection.h"

#include "objects/minimap.h"

#include "objects/coselection.h"

#include "objects/panel.h"

#include "objects/spinbox.h"

#include "objects/dropdownmenu.h"

#include "objects/multislider.h"

#include "objects/playerselection.h"

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
    // functions for swapping between the inputs
    void hideMapSelection();
    void showMapSelection();
    void hideRuleSelection();
    void showRuleSelection();
    void hidePlayerSelection();
    void showPlayerSelection();
signals:
    void buttonBack();
    void buttonNext();
    void buttonStartGame();
public slots:
    // slots for changing the map
    void mapSelectionItemClicked(QString item);
    void mapSelectionItemChanged(QString item);

    // slots for changing the rules
    void startWeatherChanged(qint32 value);
    void weatherChancesChanged();

    // general slots
    virtual void slotButtonBack();
    virtual void slotButtonNext();
    virtual void startGame();
protected:

    // buttons
    oxygine::spButton m_pButtonNext;
    oxygine::spButton m_pButtonStart;
    GameMap* m_pCurrentMap{nullptr};
    // map selection
    spMapSelection m_pMapSelection;
    spPanel m_MapInfo;
    oxygine::spTextField m_MapName;
    oxygine::spTextField m_MapAuthor;
    oxygine::spTextField m_MapDescription;
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_MinimapSlider;
    oxygine::spBox9Sprite m_pMiniMapBox;
    QVector<oxygine::spTextField> m_BuildingCountTexts;
    oxygine::spBox9Sprite m_pBuildingBackground;
    /**
     * @brief m_currentMapFile stores the current map info
     */
    QFileInfo m_currentMapFile;

    // rule selection
    spPanel m_pRuleSelection;
    spMultislider m_pWeatherSlider;

    spPlayerSelection m_pPlayerSelection;

    MapSelectionStep m_MapSelectionStep{MapSelectionStep::selectMap};
};

#endif // MAPSELECTIONMENUE_H

