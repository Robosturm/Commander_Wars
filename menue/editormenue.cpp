#include <QFile>

#include "menue/editormenue.h"

#include "coreengine/mainapp.h"

#include "coreengine/pathfindingsystem.h"

#include "menue/mainwindow.h"

#include "resource_management/movementtablemanager.h"

#include "coreengine/console.h"

#include "objects/filedialog.h"

#include "objects/mapeditdialog.h"

#include "game/terrainfindingsystem.h"
#include "game/co.h"

EditorMenue::EditorMenue()
    : InGameMenue (20, 20)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    m_EditorSelection = new EditorSelection();
    this->addChild(m_EditorSelection);
    m_Topbar = new Topbar(0, pMainapp->getSettings()->getWidth() -  m_EditorSelection->getWidth());
    this->addChild(m_Topbar);

    m_Topbar->addGroup(tr("Menu"));
    m_Topbar->addGroup(tr("Map Info"));
    m_Topbar->addItem(tr("Save Map"), "SAVEMAP", 0);
    m_Topbar->addItem(tr("Load Map"), "LOADMAP", 0);
    m_Topbar->addItem(tr("Import CoW Txt Map"), "IMPORTCOWTXT", 0);
    m_Topbar->addItem(tr("Exit Editor"), "EXIT", 0);

    m_Topbar->addItem(tr("New Map"), "NEWMAP", 1);
    m_Topbar->addItem(tr("Edit Map"), "EDITMAP", 1);
    m_Topbar->addItem(tr("Flip Map X"), "FLIPX", 1);
    m_Topbar->addItem(tr("Flip Map Y"), "FLIPY", 1);
    m_Topbar->addItem(tr("Rotate Map X"), "ROTATEX", 1);
    m_Topbar->addItem(tr("Rotate Map Y"), "ROTATEY", 1);
    m_Topbar->addItem(tr("Random Map"), "RANDOMMAP", 1);


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
    connect(pMainapp, SIGNAL(sigKeyDown(SDL_Event*)), this, SLOT(KeyInput(SDL_Event*)));
    connect(m_Topbar.get(), SIGNAL(sigItemClicked(QString)), this, SLOT(clickedTopbar(QString)));
}

void EditorMenue::clickedTopbar(QString itemID)
{
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
        spMapEditDialog mapEditDialog = new MapEditDialog("", 20, 20, 2);
        this->addChild(mapEditDialog);
        connect(mapEditDialog.get(), SIGNAL(editFinished(QString, qint32, qint32, qint32)), this, SLOT(newMap(QString, qint32, qint32, qint32)), Qt::QueuedConnection);
        this->addChild(mapEditDialog);
    }
    else if (itemID == "EDITMAP")
    {
        GameMap* pGameMap = GameMap::getInstance();
        spMapEditDialog mapEditDialog = new MapEditDialog(pGameMap->getMapName(), pGameMap->getMapWidth(), pGameMap->getMapHeight(), pGameMap->getPlayerCount());
        connect(mapEditDialog.get(), SIGNAL(editFinished(QString, qint32, qint32, qint32)), this, SLOT(changeMap(QString, qint32, qint32, qint32)), Qt::QueuedConnection);
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
}

void EditorMenue::KeyInput(SDL_Event *event)
{
    SDL_Keycode cur = event->key.keysym.sym;
    switch (cur)
    {
        case SDLK_ESCAPE:
        {
            Console::print("Leaving Editor Menue", Console::eDEBUG);
            oxygine::getStage()->addChild(new Mainwindow());
            oxygine::Actor::detach();
        }
        default:
        {
            // do nothing
            break;
        }
    }
}

void EditorMenue::cursorMoved(qint32 x, qint32 y)
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
}

void EditorMenue::onMapClickedRight()
{
    // resolve click
    GameMap* pMap = GameMap::getInstance();
    QString terrainID = pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY())->getTerrainID();
    m_EditorSelection->selectTerrain(terrainID);
}

void EditorMenue::onMapClickedLeft()
{
    // resolve click
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
}

bool EditorMenue::canTerrainBePlaced(qint32 x, qint32 y)
{
    QString terrainID = m_EditorSelection->getCurrentTerrainID();
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        if (pMap->canBePlaced(terrainID, x, y))
        {
            if (pMap->getTerrain(x, y)->getTerrainID() != terrainID)
            {
                return true;
            }
        }
    }
    return false;
}

bool EditorMenue::canBuildingBePlaced(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
        return pCurrentBuilding->canBuildingBePlaced(pMap->getTerrain(x, y));
    }
    return false;
}

bool EditorMenue::canUnitBePlaced(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
        QString movementType = m_EditorSelection->getCurrentSpUnit()->getMovementType();
        if (pMovementTableManager->getBaseMovementPoints(movementType, pMap->getTerrain(x, y)) > 0)
        {
            return true;
        }
    }
    return false;
}

void EditorMenue::placeTerrain(qint32 x, qint32 y)
{
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
}

void EditorMenue::placeBuilding(qint32 x, qint32 y)
{
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
}

void EditorMenue::placeUnit(qint32 x, qint32 y)
{
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
            GameMap* pMap = GameMap::getInstance();
            pMap->getTerrain(curX, curY)->setUnit(pUnit);
        }
    }
}

void EditorMenue::saveMap(QString filename)
{
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        GameMap* pMap = GameMap::getInstance();
        pMap->serialize(stream);
        file.close();
    }
}

void EditorMenue::loadMap(QString filename)
{
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        if (file.exists())
        {
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            GameMap::getInstance()->deserialize(stream);
            file.close();
            GameMap::getInstance()->updateSprites();
            m_EditorSelection->createPlayerSelection();
        }
    }
}

void EditorMenue::importCoWTxTMap(QString filename)
{
    if (filename.endsWith(".txt"))
    {
        QFile file(filename);
        if (file.exists())
        {
            GameMap::getInstance()->importTxtMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
}

void EditorMenue::newMap(QString mapName, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount)
{
    GameMap* pMap = GameMap::getInstance();
    pMap->setMapName(mapName);
    pMap->newMap(mapWidth, mapHeigth, playerCount);
    m_EditorSelection->createPlayerSelection();
}

void EditorMenue::changeMap(QString mapName, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount)
{
    GameMap* pMap = GameMap::getInstance();
    pMap->setMapName(mapName);
    pMap->changeMap(mapWidth, mapHeigth, playerCount);
    m_EditorSelection->createPlayerSelection();
}
