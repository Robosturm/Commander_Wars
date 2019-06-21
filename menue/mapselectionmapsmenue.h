#ifndef MAPSELECTIONMAPSMENUE_H
#define MAPSELECTIONMAPSMENUE_H

#include <QObject>

#include "QFileInfo"

#include "oxygine-framework.h"

#include "objects/mapselectionview.h"

#include "objects/minimap.h"

#include "objects/coselection.h"

#include "objects/panel.h"

#include "objects/spinbox.h"

#include "objects/dropdownmenu.h"

#include "objects/multislider.h"

#include "objects/playerselection.h"

#include "game/campaign.h"

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

    explicit MapSelectionMapsMenue(qint32 heigth = -1, spMapSelectionView pMapSelectionView = nullptr);
    virtual ~MapSelectionMapsMenue();
    // functions for swapping between the inputs
    virtual void hideMapSelection();
    virtual void showMapSelection();
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

    // general slots
    virtual void slotButtonBack();
    virtual void slotButtonNext();
    /**
     * @brief startGame starts a game for a single player mode
     */
    virtual void startGame();
protected:

    // buttons
    oxygine::spButton m_pButtonNext;
    oxygine::spButton m_pButtonStart;
    oxygine::spButton m_pButtonBack;
    spMapSelectionView m_pMapSelectionView;
    // rule selection
    spPanel m_pRuleSelection;

    spPlayerSelection m_pPlayerSelection;

    MapSelectionStep m_MapSelectionStep{MapSelectionStep::selectMap};
};

#endif // MAPSELECTIONMENUE_H

