#include <QFile>

#include "menue/editormenue.h"

#include "coreengine/mainapp.h"

#include "coreengine/pathfindingsystem.h"

#include "menue/mainwindow.h"

#include "resource_management/movementtablemanager.h"

#include "coreengine/console.h"

#include "objects/filedialog.h"

#include "objects/mapeditdialog.h"

#include "objects/dialogmodifyunit.h"

#include "objects/playerselectiondialog.h"

#include "game/terrainfindingsystem.h"
#include "game/co.h"

EditorMenue* EditorMenue::m_pInstance = nullptr;

EditorMenue::EditorMenue()
    : InGameMenue (20, 20)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    m_pInstance = this;

    m_EditorSelection = new EditorSelection();
    this->addChild(m_EditorSelection);
    m_Topbar = new Topbar(0, pApp->getSettings()->getWidth() -  m_EditorSelection->getWidth());
    this->addChild(m_Topbar);

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/mapeditor");
    pApp->getAudioThread()->playRandom();

    m_Topbar->addGroup(tr("Menu"));
    m_Topbar->addItem(tr("Save Map"), "SAVEMAP", 0);
    m_Topbar->addItem(tr("Load Map"), "LOADMAP", 0);
    m_Topbar->addItem(tr("Exit Editor"), "EXIT", 0);

    m_Topbar->addGroup(tr("Map Info"));
    m_Topbar->addItem(tr("New Map"), "NEWMAP", 1);
    m_Topbar->addItem(tr("Edit Map"), "EDITMAP", 1);
    // m_Topbar->addItem(tr("Optimize Players"), "OPTIMIZEPLAYERS", 1);
    m_Topbar->addItem(tr("Flip Map X"), "FLIPX", 1);
    m_Topbar->addItem(tr("Flip Map Y"), "FLIPY", 1);
    m_Topbar->addItem(tr("Rotate Map X"), "ROTATEX", 1);
    m_Topbar->addItem(tr("Rotate Map Y"), "ROTATEY", 1);
    m_Topbar->addItem(tr("Random Map"), "RANDOMMAP", 1);

    m_Topbar->addGroup(tr("Editor Commands"));
    m_Topbar->addItem(tr("Place Selection"), "PLACESELECTION", 2);
    m_Topbar->addItem(tr("Delete Units"), "DELETEUNITS", 2);
    m_Topbar->addItem(tr("Edit Units"), "EDITUNITS", 2);
    m_Topbar->addItem(tr("Edit Players"), "EDITPLAYERS", 2);
    m_Topbar->addItem(tr("Optimize Players"), "OPTIMIZEPLAYERS", 2);

    m_Topbar->addGroup(tr("Import/Export"));
    m_Topbar->addItem(tr("Import CoW Txt Map"), "IMPORTCOWTXT", 3);

    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigOnMapClickedLeft();
            }
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigOnMapClickedRight();
            }
            else
            {
                // ignore case
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->getPointer()->isPressed(oxygine::MouseButton::MouseButton_Left))
            {
                emit sigOnMapClickedLeft();
            }
            else
            {
                // ignore case
            }
        }
    });

    // connecting stuff
    connect(this, SIGNAL(sigOnMapClickedLeft()), this, SLOT(onMapClickedLeft()), Qt::QueuedConnection);
    connect(this, SIGNAL(sigOnMapClickedRight()), this, SLOT(onMapClickedRight()), Qt::QueuedConnection);
    connect(m_Cursor.get(), SIGNAL(sigCursorMoved(qint32,qint32)), this, SLOT(cursorMoved(qint32,qint32)), Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyDown, this, &EditorMenue::KeyInput, Qt::QueuedConnection);
    connect(m_Topbar.get(), SIGNAL(sigItemClicked(QString)), this, SLOT(clickedTopbar(QString)), Qt::QueuedConnection);
}

EditorMenue::~EditorMenue()
{
    m_pInstance = nullptr;
}

void EditorMenue::clickedTopbar(QString itemID)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (itemID == "EXIT")
    {
        Console::print("Leaving Editor Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new Mainwindow());
        oxygine::Actor::detach();
    }
    else if (itemID == "SAVEMAP")
    {
        QVector<QString> wildcards;
        wildcards.append("*.map");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog saveDialog = new FileDialog(path, wildcards, GameMap::getInstance()->getMapName());
        this->addChild(saveDialog);
        connect(saveDialog.get(), SIGNAL(sigFileSelected(QString)), this, SLOT(saveMap(QString)), Qt::QueuedConnection);
    }
    else if (itemID == "LOADMAP")
    {
        QVector<QString> wildcards;
        wildcards.append("*.map");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog saveDialog = new FileDialog(path, wildcards);
        this->addChild(saveDialog);
        connect(saveDialog.get(), SIGNAL(sigFileSelected(QString)), this, SLOT(loadMap(QString)), Qt::QueuedConnection);
    }
    else if (itemID == "IMPORTCOWTXT")
    {
        QVector<QString> wildcards;
        wildcards.append("*.txt");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog saveDialog = new FileDialog(path, wildcards);
        this->addChild(saveDialog);
        connect(saveDialog.get(), SIGNAL(sigFileSelected(QString)), this, SLOT(importCoWTxTMap(QString)), Qt::QueuedConnection);
    }
    else if (itemID == "NEWMAP")
    {
        spMapEditDialog mapEditDialog = new MapEditDialog("", "", "", 20, 20, 2);
        this->addChild(mapEditDialog);
        connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::newMap, Qt::QueuedConnection);
        this->addChild(mapEditDialog);
    }
    else if (itemID == "EDITMAP")
    {
        GameMap* pGameMap = GameMap::getInstance();
        spMapEditDialog mapEditDialog = new MapEditDialog(pGameMap->getMapName(), pGameMap->getMapAuthor(), pGameMap->getMapDescription(),
                                                          pGameMap->getMapWidth(), pGameMap->getMapHeight(), pGameMap->getPlayerCount());
        connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::changeMap, Qt::QueuedConnection);
        this->addChild(mapEditDialog);
    }
    else if (itemID == "FLIPX")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->flipX();
    }
    else if (itemID == "FLIPY")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->flipY();
    }
    else if (itemID == "ROTATEX")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->rotateX();
    }
    else if (itemID == "ROTATEY")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->rotateY();
    }
    else if (itemID == "RANDOMMAP")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->randomMap(30, 40, 4);
    }
    else if (itemID == "PLACESELECTION")
    {
        m_EditorMode = EditorModes::PlaceEditorSelection;
    }
    else if (itemID == "DELETEUNITS")
    {
        m_EditorMode = EditorModes::RemoveUnits;
    }
    else if (itemID == "EDITUNITS")
    {
        m_EditorMode = EditorModes::EditUnits;
    }
    else if (itemID == "OPTIMIZEPLAYERS")
    {
        optimizePlayers();
    }
    else if (itemID == "EDITPLAYERS")
    {
        spPlayerSelectionDialog pDiaolog = new PlayerSelectionDialog();
        addChild(pDiaolog);
        connect(pDiaolog.get(), &PlayerSelectionDialog::sigPlayersChanged, this, &EditorMenue::playersChanged, Qt::QueuedConnection);
    }
    pApp->continueThread();
}

void EditorMenue::playersChanged()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_EditorSelection->createPlayerSelection();
    pApp->continueThread();
}

void EditorMenue::optimizePlayers()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    QVector<bool> foundPlayers(pMap->getPlayerCount(), false);
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeigth = pMap->getMapHeight();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
            if (pBuilding != nullptr && pBuilding->getOwner() != nullptr)
            {
                foundPlayers[pBuilding->getOwner()->getPlayerID()] = true;
            }
            if (pUnit != nullptr)
            {
                foundPlayers[pUnit->getOwner()->getPlayerID()] = true;
            }
        }
    }
    for (qint32 i = foundPlayers.size() - 1; i >= 0; i--)
    {
        if (pMap->getPlayerCount() > 2)
        {
            if (foundPlayers[i] == false)
            {
                pMap->removePlayer(i);
            }
        }
    }
    m_EditorSelection->createPlayerSelection();
    pApp->continueThread();
}

void EditorMenue::KeyInput(SDL_Event event)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    SDL_Keycode cur = event.key.keysym.sym;
    switch (cur)
    {
        case SDLK_ESCAPE:
        {
            Console::print("Leaving Editor Menue", Console::eDEBUG);
            oxygine::getStage()->addChild(new Mainwindow());
            oxygine::Actor::detach();
            break;
        }
        default:
        {
            // do nothing
            break;
        }
    }
    pApp->continueThread();
}

void EditorMenue::cursorMoved(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    switch (m_EditorMode)
    {
        case EditorModes::RemoveUnits:
        {
            m_Cursor->changeCursor("cursor+delete");
            break;
        }
        case EditorModes::EditUnits:
        {
            m_Cursor->changeCursor("cursor+edit");
            break;
        }
        case EditorModes::PlaceEditorSelection:
        {
            // resolve cursor move
            switch (m_EditorSelection->getCurrentMode())
            {
                case EditorSelection::EditorMode::Terrain:
                {
                    if (canTerrainBePlaced(x, y))
                    {
                        m_Cursor->changeCursor("cursor+default");
                    }
                    else
                    {
                        m_Cursor->changeCursor("cursor+unable");
                    }
                    break;
                }
                case EditorSelection::EditorMode::Building:
                {
                    if (canBuildingBePlaced(x, y))
                    {
                        m_Cursor->changeCursor("cursor+default");
                    }
                    else
                    {
                        m_Cursor->changeCursor("cursor+unable");
                    }
                    break;
                }
                case EditorSelection::EditorMode::Unit:
                {
                    if (canUnitBePlaced(x, y))
                    {
                        m_Cursor->changeCursor("cursor+default");
                    }
                    else
                    {
                        m_Cursor->changeCursor("cursor+unable");
                    }
                    break;
                }
            }
            break;
        }
    }
    pApp->continueThread();
}

void EditorMenue::onMapClickedRight()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // resolve click
    GameMap* pMap = GameMap::getInstance();
    switch (m_EditorSelection->getCurrentMode())
    {
        case EditorSelection::EditorMode::Terrain:
        {
            QString terrainID = pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY())->getTerrainID();
            m_EditorSelection->selectTerrain(terrainID);
            break;
        }
        case EditorSelection::EditorMode::Building:
        {
            Building* pBuilding = pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY())->getBuilding();
            if (pBuilding != nullptr)
            {
                m_EditorSelection->selectBuilding(pBuilding->getBuildingID());
            }
            break;
        }
        case EditorSelection::EditorMode::Unit:
        {
            Unit* pUnit = pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY())->getUnit();
            if (pUnit != nullptr)
            {
                m_EditorSelection->selectUnit(pUnit->getUnitID());
            }
            break;
        }
    }
    m_EditorMode = EditorModes::PlaceEditorSelection;
    pApp->continueThread();
}

void EditorMenue::onMapClickedLeft()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // resolve click
    switch (m_EditorMode)
    {
        case EditorModes::RemoveUnits:
        {
            Unit* pUnit = GameMap::getInstance()->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY())->getUnit();
            if (pUnit != nullptr)
            {
                pUnit->killUnit();
            }
            break;
        }
        case EditorModes::EditUnits:
        {
            Unit* pUnit = GameMap::getInstance()->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY())->getUnit();
            if (pUnit != nullptr)
            {
                addChild(new DialogModifyUnit(pUnit));
            }
            break;
        }
        case EditorModes::PlaceEditorSelection:
        {
            switch (m_EditorSelection->getCurrentMode())
            {
                case EditorSelection::EditorMode::Terrain:
                {
                    placeTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    break;
                }
                case EditorSelection::EditorMode::Building:
                {
                    placeBuilding(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    break;
                }
                case EditorSelection::EditorMode::Unit:
                {
                    placeUnit(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    break;
                }
            }
            break;
        }
    }
    pApp->continueThread();
}

bool EditorMenue::canTerrainBePlaced(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    bool ret = false;
    QString terrainID = m_EditorSelection->getCurrentTerrainID();
    GameMap* pMap = GameMap::getInstance();

    if (pMap->onMap(x, y))
    {
        if (pMap->canBePlaced(terrainID, x, y))
        {
            if (pMap->getTerrain(x, y)->getTerrainID() != terrainID ||
                m_EditorSelection->getSizeMode() == EditorSelection::PlacementSize::Small)
            {
                ret = true;
            }
        }
    }
    pApp->continueThread();
    return ret;
}

bool EditorMenue::canBuildingBePlaced(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    bool ret = false;
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
        ret = pCurrentBuilding->canBuildingBePlaced(pMap->getTerrain(x, y));
    }
    pApp->continueThread();
    return ret;
}

bool EditorMenue::canUnitBePlaced(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    bool ret = false;
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
        QString movementType = m_EditorSelection->getCurrentSpUnit()->getMovementType();
        if (pMovementTableManager->getBaseMovementPoints(movementType, pMap->getTerrain(x, y)) > 0)
        {
            ret = true;
        }
    }
    pApp->continueThread();
    return ret;
}

void EditorMenue::placeTerrain(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QVector<QPoint> points;
    GameMap* pMap = GameMap::getInstance();
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(x, y, 0, 1);
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            TerrainFindingSystem Pfs(pMap->getTerrain(x, y)->getID(),x , y);
            Pfs.explore();
            points = Pfs.getAllNodePoints();
            break;
        }
    }
    for (qint32 i = 0; i < points.size(); i++)
    {
        // nice we can place the terrain
        if (canTerrainBePlaced(points.at(i).x(), points.at(i).y()))
        {
            QString terrainID = m_EditorSelection->getCurrentTerrainID();

            Mainapp* pApp = Mainapp::getInstance();
            QString function1 = "useTerrainAsBaseTerrain";
            QJSValueList args1;
            QJSValue useTerrainAsBaseTerrain = pApp->getInterpreter()->doFunction(terrainID, function1, args1);
            if (points.size() < 14)
            {
                pMap->replaceTerrain(terrainID, points.at(i).x(), points.at(i).y(), useTerrainAsBaseTerrain.toBool(), true);
            }
            else
            {
                pMap->replaceTerrain(terrainID, points.at(i).x(), points.at(i).y(), useTerrainAsBaseTerrain.toBool(), false);
            }
        }
    }
    if (points.size() >= 14)
    {
        pMap->updateSprites();
    }
    pApp->continueThread();
}

void EditorMenue::placeBuilding(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    GameMap* pMap = GameMap::getInstance();
    QVector<QPoint> points;
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(x, y, 0, 1);
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            TerrainFindingSystem Pfs(pMap->getTerrain(x, y)->getID(),x , y);
            Pfs.explore();
            points = Pfs.getAllNodePoints();
            break;
        }
    }
    for (qint32 i = 0; i < points.size(); i++)
    {
        // point still on the map great :)
        qint32 curX = points.at(i).x();
        qint32 curY = points.at(i).y();
        if (canBuildingBePlaced(curX, curY))
        {
            spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
            Building* pBuilding = new Building(pCurrentBuilding->getBuildingID());
            pBuilding->setOwner(pCurrentBuilding->getOwner());
            pMap->getTerrain(curX, curY)->setBuilding(pBuilding);
        }
    }
    pApp->continueThread();
}

void EditorMenue::placeUnit(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    QVector<QPoint> points;
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(x, y, 0, 1);
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
    }
    for (qint32 i = 0; i < points.size(); i++)
    {
        // point still on the map great :)
        qint32 curX = points.at(i).x();
        qint32 curY = points.at(i).y();
        if (canUnitBePlaced(curX, curY))
        {
            spUnit pCurrentUnit = m_EditorSelection->getCurrentSpUnit();
            spUnit pUnit = new Unit(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner());
            pUnit->setAiMode(GameEnums::GameAi::GameAi_Normal);
            GameMap* pMap = GameMap::getInstance();
            pMap->getTerrain(curX, curY)->setUnit(pUnit);
        }
    }
    pApp->continueThread();
}

void EditorMenue::saveMap(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        GameMap* pMap = GameMap::getInstance();
        pMap->serializeObject(stream);
        file.close();
    }
    pApp->continueThread();
}

void EditorMenue::loadMap(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        if (file.exists())
        {
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            GameMap::getInstance()->deserializeObject(stream);
            file.close();
            GameMap* pMap = GameMap::getInstance();
            pMap->updateSprites();
            pMap->centerMap(pMap->getMapWidth() / 2, pMap->getMapHeight() / 2);
            m_EditorSelection->createPlayerSelection();
        }
    }
    pApp->continueThread();
}

void EditorMenue::importCoWTxTMap(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    if (filename.endsWith(".txt"))
    {
        QFile file(filename);
        if (file.exists())
        {
            GameMap::getInstance()->importTxtMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    pApp->continueThread();
}

void EditorMenue::newMap(QString mapName, QString mapAuthor, QString mapDescription, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    GameMap* pMap = GameMap::getInstance();
    pMap->setMapName(mapName);
    pMap->setMapAuthor(mapAuthor);
    pMap->setMapDescription(mapDescription);
    pMap->newMap(mapWidth, mapHeigth, playerCount);
    m_EditorSelection->createPlayerSelection();
    pApp->continueThread();
}

void EditorMenue::changeMap(QString mapName, QString mapAuthor, QString mapDescription, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    GameMap* pMap = GameMap::getInstance();
    pMap->setMapName(mapName);
    pMap->setMapAuthor(mapAuthor);
    pMap->setMapDescription(mapDescription);
    pMap->changeMap(mapWidth, mapHeigth, playerCount);
    m_EditorSelection->createPlayerSelection();

    pApp->continueThread();
}
