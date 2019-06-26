#ifndef EDITORMENUE_H
#define EDITORMENUE_H

#include "menue/ingamemenue.h"

#include <QObject>

#include "objects/editorselection.h"

#include "objects/topbar.h"

class EditorMenue : public InGameMenue
{
    Q_OBJECT
public:
    enum class EditorModes
    {
        PlaceEditorSelection,
        RemoveUnits,
        EditUnits,
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
signals:
    void sigOnMapClickedLeft();
    void sigOnMapClickedRight();
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
    void KeyInput(SDL_Event event);
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
    void newMap(QString mapName, QString mapAuthor, QString mapDescription, QString scriptFile, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount);
    /**
     * @brief changeMap
     * @param mapName
     * @param mapWidth
     * @param mapHeigth
     * @param playerCount
     */
    void changeMap(QString mapName, QString mapAuthor, QString mapDescription, QString scriptFile, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount);
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
     * @param forestchance
     * @param mountainChance
     * @param seachance
     * @param buildingchance
     */
    void createRandomMap(QString mapName, QString author, QString description,
                         qint32 width,qint32 heigth, qint32 playerCount,
                         bool roadSupport, qint32 seed,
                         float forestchance, float mountainChance, float seachance, float buildingchance,
                         float factoryChance, float airPortChance, float harbourChance);
private:
    spEditorSelection m_EditorSelection{nullptr};
    EditorModes m_EditorMode{EditorModes::PlaceEditorSelection};
    spTopbar m_Topbar;
    oxygine::spTextField xyTextInfo;

    static EditorMenue* m_pInstance;
};

#endif // EDITORMENUE_H
