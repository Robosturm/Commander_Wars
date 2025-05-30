#pragma once

#include <QObject>
#include <QTemporaryDir>

#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "coreengine/settings.h"

#include "menue/basegamemenu.h"

#include "gameinput/humanplayerinput.h"

#include "objects/dialogs/editor/mapeditdialog.h"
#include "objects/editorselection.h"
#include "objects/base/topbar.h"
#include "objects/base/label.h"

#include "awbwReplayReader/awbwmapdownloader.h"

class EditorMenue;
using spEditorMenue = std::shared_ptr<EditorMenue>;

class EditorMenue final : public BaseGamemenu
{
    Q_OBJECT
public:
    enum class EditorModes
    {
        PlaceEditorSelection,
        RemoveUnits,
        EditUnits,
        EditTerrain,
        CopySelection,
    };

    enum class CursorModes
    {
        Rect,
        Circle,
    };
    struct PlacingState
    {
        bool active{false};
        bool placePressed{false};
        qint32 x{-1};
        qint32 y{-1};
    };

    explicit EditorMenue();
    virtual ~EditorMenue();
    /**
     * @brief cleanTemp clean ups undo and redo steps
     * @param step
     */
    void cleanTemp(qint32 step);
    /**
     * @brief createTempFile
     */
    void createTempFile(bool cleanUp = true);
    /**
     * @brief undo
     */
    void editorUndo();
    /**
     * @brief redo
     */
    void editorRedo();
signals:
    void sigOnMapClickedLeft();
    void sigOnMapClickedRight();
    void sigResizeMap(qint32 left, qint32 top, qint32 right, qint32 bottom);
public slots:
    /**
     * @brief focusEditor
     */
    void focusEditor();
    /**
     * @brief viewMapStats
     */
    void viewMapStats();
    /**
     * @brief showEditBiomes
     */
    void showEditBiomes();
    /**
     * @brief updateSprites
     */
    void updateSprites();
    /**
     * @brief toggleMiddleCrossGrid
     */
    void toggleMiddleCrossGrid();
    /**
     * @brief toggleGridLayout
     */
    void toggleGridLayout();
    /**
     * @brief optimizePlayers
     */
    void optimizePlayers();
    /**
     * @brief scriptFinished
     */
    void scriptFinished();
    /**
     * @brief campaignFinished
     */
    void campaignFinished();
    /**
     * @brief editFinishedCanceled
     */
    void editFinishedCanceled();
    /**
     * @brief cursorMoved called when the cursor moved
     */
    void cursorMoved(qint32 x, qint32 y);
    /**
     * @brief onMapClickedLeft left click on the map
     */
    void onMapClickedLeft(qint32 x, qint32 y);
    /**
     * @brief onMapClickedLeftUp
     * @param x
     * @param y
     */
    void onMapClickedLeftUp(qint32 x, qint32 y);
    /**
     * @brief onMapClickedLeftDown
     * @param x
     * @param y
     */
    void onMapClickedLeftDown(qint32 x, qint32 y);
    /**
     * @brief onMapClickedRight right click on the map
     */
    void onMapClickedRight(qint32 x, qint32 y);
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
    /**
     * @brief clickedTopbar event handling for the top bar
     * @param itemID
     */
    void clickedTopbar(QString itemID);
    /**
     * @brief saveMap saves the map into the named file
     * @param file
     */
    void saveMap(QString filename);
    /**
     * @brief loadMap loads the map from the named file
     * @param file
     */
    void loadMap(QString filename);
    /**
     * @brief importCoWTxTMap
     * @param filename
     */
    void importCoWTxTMap(QString filename);
    /**
     * @brief importAWDSAwsMap
     * @param filename
     */
    void importAWDSAwsMap(QString filename);
    /**
     * @brief exportAWDSAwsMap
     * @param filename
     */
    void exportAWDSAwsMap(QString filename);
    /**
     * @brief importAW4Aw4Map
     * @param filename
     */
    void importAW4Aw4Map(QString filename);
    /**
     * @brief importAWByWeb
     * @param filename
     */
    void importAWByWeb(QString filename);
    /**
     * @brief playersChanged
     */
    void playersChanged();
    /**
     * @brief rulesChanged
     */
    void rulesChanged();
    /**
     * @brief showResizeMap
     */
    void showResizeMap();
    /**
     * @brief resizeMap
     * @param left
     * @param top
     * @param right
     * @param bottom
     */
    void resizeMap(qint32 left, qint32 top, qint32 right, qint32 bottom);
    /**
     * @brief selectionChanged
     */
    void selectionChanged();
    /**
     * @brief exitEditor
     */
    void exitEditor();
    /**
     * @brief autosave
     */
    void autosave();
    /**
     * @brief showSaveMap
     */
    void showSaveMap();
    /**
     * @brief showLoadMap
     */
    void showLoadMap();
    /**
     * @brief showEditScript
     */
    void showEditScript();
    /**
     * @brief showEditCampaign
     */
    void showEditCampaign();
    /**
     * @brief showImportCoWTxTMap
     */
    void showImportCoWTxTMap();
    /**
     * @brief showImportAwdsAws
     */
    void showImportAwdsAws();
    /**
     * @brief showExportAwdsAws
     */
    void showExportAwdsAws();
    /**
     * @brief showImportAwdsAw4
     */
    void showImportAwdsAw4();
    /**
     * @brief showImportAwByWeb
     */
    void showImportAwByWeb();
    /**
     * @brief showNewMap
     */
    void showNewMap();
    /**
     * @brief showEditMap
     */
    void showEditMap();
    /**
     * @brief showExtendMap
     */
    void showExtendMap();
    /**
     * @brief extendMap
     * @param mapFile
     * @param direction
     */
    void extendMap(const QString & mapFile, GameEnums::Directions direction);
    /**
     * @brief flipX
     */
    void flipX();
    /**
     * @brief flipY
     */
    void flipY();
    /**
     * @brief rotateX
     */
    void rotateX();
    /**
     * @brief rotateY
     */
    void rotateY();
    /**
     * @brief rotateX90
     */
    void rotateX90();
    /**
     * @brief rotateY90
     */
    void rotateY90();
    /**
     * @brief rotate90
     */
    void rotate90();
    /**
     * @brief showRandomMap
     */
    void showRandomMap();
    /**
     * @brief changePlaceSelection
     */
    void changePlaceSelection();
    /**
     * @brief deleteUnits
     */
    void deleteUnits();
    /**
     * @brief editUnits
     */
    void editUnits();
    /**
     * @brief editTerrains
     */
    void editTerrains();
    /**
     * @brief showEditPlayers
     */
    void showEditPlayers();
    /**
     * @brief showEditRules
     */
    void showEditRules();
    /**
     * @brief showExit
     */
    void showExit();
    /**
     * @brief copy
     */
    void copy();
    /**
     * @brief showImportAwByWebByMapId
     */
    void showImportAwByWebByMapId();
    /**
     * @brief downloadAwbwMapById
     * @param mapId
     */
    void downloadAwbwMapById(quint32 mapId);
protected slots:
    /**
     * @brief KeyInput called on any key input
     * @param event
     */
    virtual void keyInput(oxygine::KeyEvent event) override;
    /**
     * @brief newMap
     * @param info
     */
    void newMap(MapEditDialog::MapEditInfo info);
    /**
     * @brief changeMap
     * @param info
     */
    void changeMap(MapEditDialog::MapEditInfo info);
    /**
     * @brief pasteSelection
     */
    void pasteSelection(qint32 x, qint32 y, bool click, EditorSelection::EditorMode selection);
    /**
     * @brief createMarkedArea
     * @param pActor
     * @param p1
     * @param p2
     * @param mode
     */
    void createMarkedArea(oxygine::spActor pActor, QPoint p1, QPoint p2, EditorMenue::CursorModes mode, QColor color = Qt::white);
    /**
     * @brief createRandomMap
     * @param mapName
     * @param author
     * @param description
     * @param width
     * @param heigth
     * @param playerCount
     * @param roadSupport
     * @param seed
     * @param terrains
     * @param buildings
     * @param ownedBaseSize
     * @param startBaseSize
     */
    void createRandomMap(QString mapName, QString author, QString description,
                         qint32 width, qint32 heigth, qint32 playerCount,
                         bool roadSupport, qint32 seed,
                         QVector<std::tuple<QString, float>> terrains,
                         QVector<std::tuple<QString, float>> buildings,
                         QVector<float> ownedBaseSize,
                         float startBaseSize,
                         QVector<std::tuple<QString, float>> units,
                         qint32 unitCount,
                         float startBaseUnitSize,
                         QVector<float> unitDistribution,
                         bool unitsDistributed,
                         bool mirrored);
    /**
     * @brief onEnter
     */
    virtual void onEnter() override;
    /**
     * @brief onAwbwMapDownloadResult
     */
    void onAwbwMapDownloadResult(bool success);

private:
    void updateGrids();
    void getSquareTiles(QVector<QPoint> & points, QPoint start, QPoint end, QPoint currentPos);
private:
    spEditorSelection m_EditorSelection{nullptr};
    EditorModes m_EditorMode{EditorModes::PlaceEditorSelection};
    spTopbar m_Topbar;
    spLabel m_xyTextInfo;
    PlacingState m_placingState;

    qint32 m_tempCounter{0};

    QRect m_copyRect{-1, -1, 0, 0};
    oxygine::spActor m_copyRectActor{MemoryManagement::create<oxygine::Actor>()};
    oxygine::spActor m_cursorActor{MemoryManagement::create<oxygine::Actor>()};
    bool m_gridVisible{false};
    bool m_middleCrossGridVisible{false};
    QTimer m_autosaveTimer;
    spHumanPlayerInput m_HumanInput;
    AwbwMapDownloader m_awbwMapDownloader;

    QTemporaryDir m_tempDir = Settings::getInstance()->newTempDir();
};

Q_DECLARE_INTERFACE(EditorMenue, "EditorMenue");
