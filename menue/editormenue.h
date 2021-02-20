#ifndef EDITORMENUE_H
#define EDITORMENUE_H

#include <QObject>
#include "oxygine/KeyEvent.h"

#include "menue/ingamemenue.h"

#include "objects/editorselection.h"
#include "objects/base/topbar.h"
#include "objects/base/label.h"

#include "coreengine/LUPDATE_MACROS.h"

class EditorMenue : public InGameMenue
{
    Q_OBJECT
public:
    ENUM_CLASS EditorModes
    {
        PlaceEditorSelection,
        RemoveUnits,
        EditUnits,
        EditTerrain,
        CopySelection,
    };

    ENUM_CLASS CursorModes
    {
        Rect,
        Circle,
    };

    explicit EditorMenue();
    virtual ~EditorMenue();

    static inline EditorMenue* getInstance()
    {
        return m_pInstance;
    }
    /**
     * @brief optimizePlayers
     */
    void optimizePlayers();
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
     * @brief KeyInput called on any key input
     * @param event
     */
    void KeyInput(oxygine::KeyEvent event);
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
     * @brief importAWDCAw4Map
     * @param filename
     */
    void importAWDCAw4Map(QString filename);
    /**
     * @brief importAWByWeb
     * @param filename
     */
    void importAWByWeb(QString filename);
    /**
     * @brief newMap
     * @param mapName
     * @param mapWidth
     * @param mapHeigth
     * @param playerCount
     */
    void newMap(QString mapName, QString author, QString description, QString scriptFile,
                qint32 mapWidth, qint32 mapHeigth, qint32 playerCount,
                qint32 turnLimit, quint32 buildLimit);
    /**
     * @brief changeMap
     * @param mapName
     * @param mapWidth
     * @param mapHeigth
     * @param playerCount
     */
    void changeMap(QString mapName, QString author, QString description, QString scriptFile,
                   qint32 mapWidth, qint32 mapHeigth, qint32 playerCount,
                   qint32 turnLimit, quint32 buildLimit);
    /**
     * @brief playersChanged
     */
    void playersChanged();
    /**
     * @brief rulesChanged
     */
    void rulesChanged();
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
                         bool unitsDistributed);
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
     * @brief createMarkedArea
     * @param pActor
     * @param p1
     * @param p2
     * @param mode
     */
    void createMarkedArea(oxygine::spActor pActor, QPoint p1, QPoint p2, CursorModes mode, QColor color = Qt::white);
    /**
     * @brief selectionChanged
     */
    void selectionChanged();
    /**
     * @brief pasteSelection
     */
    void pasteSelection(qint32 x, qint32 y, bool click, EditorSelection::EditorMode selection);
    /**
     * @brief exitEditor
     */
    void exitEditor();
    /**
     * @brief autosave
     */
    void autosave();
private:
    spEditorSelection m_EditorSelection{nullptr};
    EditorModes m_EditorMode{EditorModes::PlaceEditorSelection};
    spTopbar m_Topbar;
    spLabel xyTextInfo;

    static EditorMenue* m_pInstance;

    qint32 tempCounter{0};

    QRect copyRect{-1, -1, 0, 0};
    oxygine::spActor copyRectActor{new oxygine::Actor()};
    oxygine::spActor cursorActor{new oxygine::Actor()};

    QTimer m_autosaveTimer;
};

#endif // EDITORMENUE_H
