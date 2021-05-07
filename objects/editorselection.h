#ifndef EDITORSELECTION_H
#define EDITORSELECTION_H


#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include <QObject>
#include <QVector>
#include <QVector3D>
#include "game/terrain.h"
#include "game/building.h"
#include "game/gamemap.h"
#include "game/player.h"
#include "game/unit.h"

#include "coreengine/LUPDATE_MACROS.h"

class EditorSelection;
typedef oxygine::intrusive_ptr<EditorSelection> spEditorSelection;

class EditorSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief The EditorMode enum describes what we want to place at the moment
     */
    ENUM_CLASS EditorMode
    {
        All = -1,
        Terrain = 0,
        Building,
        Unit,
    };
    /**
     * @brief The PlacementSize enum describes how terrain is placed
     */
    ENUM_CLASS PlacementSize
    {
        Small = 0,
        Medium,
        Big,
        Fill
    };

    explicit EditorSelection();


    inline EditorMode getCurrentMode() const
    {
        return m_Mode;
    }
    inline QString getCurrentTerrainID()
    {
        return m_Terrains.at(m_selectedItem)->getTerrainID();
    }
    inline QString getCurrentBuildingID()
    {
        return m_Buildings.at(m_selectedItem)->getBuildingID();
    }
    /**
     * @brief getCurrentSpBuilding the current selected unit
     * @return
     */
    spBuilding getCurrentSpBuilding();
    /**
     * @brief getCurrentSpUnit the current selected unit
     * @return
     */
    spUnit getCurrentSpUnit();
    /**
     * @brief getSizeMode
     * @return
     */
    PlacementSize getSizeMode() const;
    /**
     * @brief createPlayerSelection
     */
    void createPlayerSelection();
    /**
     * @brief KeyInput
     * @param event
     */
    void KeyInput(Qt::Key cur);

signals:
   void sigUpdateSelectedPlayer();
   void sigSelectionChanged();
   void sigChangeSelectedPlayer(qint32 player);
   void sigUpdateTerrainView();
   void sigUpdateBuildingView();
   void sigUpdateUnitView();
   void sigChangeScrollValue(qint32 dir);
public slots:
    void selectTerrain(QString terrainID);
    void selectBuilding(QString buildingID);
    void selectUnit(QString unitID);
    void updateSelectedPlayer();
    void changeSelectedPlayer(qint32 player);

    void updateTerrainView();
    void updateBuildingView();
    void updateUnitView();

    void changeScrollValue(qint32 dir);
private:
    /**
     * @brief selectBuilding
     * @param building
     */
    void selectBuilding(qint32 building);
    /**
     * @brief selectUnit
     * @param unit
     */
    void selectUnit(qint32 unit);
    /**
     * @brief selectTerrain
     * @param terrain
     */
    void selectTerrain(qint32 terrain);
    /**
     * @brief getPosX
     * @param xCounter
     * @return
     */
    qint32 getPosX(qint32 xCounter);
    /**
     * @brief initTerrainSection
     */
    void initTerrainSection();
    /**
     * @brief createTerrainSectionLabel
     * @param currentIdentifier
     * @param xCounter
     * @param posY
     */
    void createTerrainSectionLabel(qint32 item, qint32 & currentIdentifier, qint32 & xCounter, qint32 & posY);
    /**
     * @brief initBuildingSection
     */
    void initBuildingSection();
    /**
     * @brief initUnitSelection
     */
    void initUnitSelection();
    /**
     * @brief createUnitSectionLabel
     * @param item
     * @param currentIdentifier
     * @param xCounter
     * @param posY
     */
    void createUnitSectionLabel(qint32 item, qint32 & currentIdentifier, qint32 & xCounter, qint32 & posY);
    /**
     * @brief hideSelection
     */
    void hideSelection();
    /**
     * @brief changeSelection
     * @param item
     */
    void changeSelection(qint32 item);
private:
    void createBoxPlacementSize();
    void createBoxSelectionMode();
    void initSelection();

    /**
     * @brief calcMaxPlayerSelection the amount of player hq's shown in the select player part
     * @return
     */
    qint32 calcMaxPlayerSelection();
    oxygine::spSprite createV9Box(qint32 x, qint32 y, qint32 width, qint32 heigth);

private:
    // small hints for the ui
    static const qint32 m_frameSize = 30;
    static const qint32 m_selectionHeight = 80;
    static const qint32 m_startHPlacementSize = 0;
    static const qint32 m_startHSelectionType = m_startHPlacementSize + m_selectionHeight;
    static const qint32 m_startHSelectedPlayer = m_startHSelectionType + m_startHPlacementSize + m_selectionHeight;
    static const qint32 m_startHTerrain = m_startHSelectedPlayer + m_selectionHeight;
    const qint32 m_startH = GameMap::getImageSize() * 2;
    static const float m_xFactor;
    static const float m_yFactor;
    EditorMode m_Mode{EditorMode::Terrain};
    PlacementSize m_SizeMode{PlacementSize::Small};
    qint32 m_StartIndex{0};
    qint32 m_xCount{0};
    qint32 m_labelWidth{0};

    oxygine::spClipRectActor m_PlacementSelectionClip;
    oxygine::spActor m_PlacementActor;
    QVector<spTerrain> m_Terrains;
    QVector<oxygine::spActor> m_terrainActors;
    QVector<spBuilding> m_Buildings;
    QVector<spUnit> m_Units;
    QVector<oxygine::spActor> m_unitActors;

    oxygine::spSprite m_BoxPlacementSize;
    oxygine::spSprite m_BoxSelectionType;
    oxygine::spSprite m_BoxSelectedPlayer;
    oxygine::spSprite m_BoxPlacementSelection;

    oxygine::spSprite m_CurrentSelector;
    oxygine::spSprite m_CurrentSelectorSize;
    oxygine::spSprite m_CurrentSelectorMode;

    oxygine::spSprite m_pSpriteTerrainMode;
    oxygine::spSprite m_pSpriteBuildingMode;
    oxygine::spSprite m_pSpriteUnitMode;

    qint32 m_selectedItem{0};
    /**
     * @brief current selected player
     */
    spPlayer m_currentPlayer{nullptr};
    qint32 m_playerStartIndex{0};
    QVector<spBuilding> m_Players;
};

#endif // EDITORSELECTION_H


