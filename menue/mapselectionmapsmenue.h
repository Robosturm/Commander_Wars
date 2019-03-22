#ifndef MAPSELECTIONMAPSMENUE_H
#define MAPSELECTIONMAPSMENUE_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/mapselection.h"

#include "objects/minimap.h"

#include "objects/coselection.h"

#include "objects/panel.h"

#include "objects/spinbox.h"

#include "objects/dropdownmenu.h"

#include "objects/multislider.h"

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
    void hideCOSelection();
    void showCOSelection();
signals:
    void buttonBack();
    void buttonNext();
    void buttonStartGame();
    void buttonAllCOsRandom();
public slots:
    // slots for changing the map
    void mapSelectionItemClicked(QString item);
    void mapSelectionItemChanged(QString item);

    // slots for changing the rules
    void startWeatherChanged(qint32 value);
    void weatherChancesChanged();

    // slots for changing player data
    void allPlayerIncomeChanged(float value);
    void playerIncomeChanged(float value, qint32 playerIdx);
    void allPlayerStartFondsChanged(float value);
    void playerStartFondsChanged(float value, qint32 playerIdx);
    void playerTeamChanged(qint32 value, qint32 playerIdx);
    void playerColorChanged(QColor value, qint32 playerIdx);
    void playerCO1Changed(QString coid, qint32 playerIdx);
    void playerCO2Changed(QString coid, qint32 playerIdx);
    void slotAllCOsRandom();

    // general slots
    void slotButtonBack();
    void slotButtonNext();
    void startGame();
protected:

private:
    // buttons
    oxygine::spButton m_pButtonNext;
    oxygine::spButton m_pButtonStart;
    GameMap* m_pCurrentMap{nullptr};
    // map selection
    spMapSelection m_pMapSelection;
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_MinimapSlider;
    oxygine::spBox9Sprite m_pMiniMapBox;
    QVector<oxygine::spTextField> m_BuildingCountTexts;
    oxygine::spBox9Sprite m_pBuildingBackground;
    // rule selection
    spPanel m_pRuleSelection;
    spMultislider m_pWeatherSlider;

    // player selection
    spPanel m_pPlayerSelection;
    QVector<oxygine::spSprite> m_playerCO1;
    QVector<oxygine::spSprite> m_playerCO2;
    QVector<spSpinBox> m_playerIncomes;
    QVector<spSpinBox> m_playerStartFonds;
    QVector<spDropDownmenu> m_playerAIs;

    MapSelectionStep m_MapSelectionStep{MapSelectionStep::selectMap};
};

#endif // MAPSELECTIONMENUE_H

