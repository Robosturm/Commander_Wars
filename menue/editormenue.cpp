#include "menue/editormenue.h"

#include "coreengine/mainapp.h"

#include "coreengine/pathfindingsystem.h"

#include "resource_management/movementtablemanager.h"

EditorMenue::EditorMenue()
{
    this->addChild(m_EditorSelection);
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
    connect(m_Cursor.get(), SIGNAL(sigCursorMoved()), this, SLOT(cursorMoved()), Qt::QueuedConnection);
}

void EditorMenue::cursorMoved()
{
    // resolve cursor move
    switch (m_EditorSelection->getCurrentMode())
    {
        case EditorSelection::EditorMode::Terrain:
        {
            if (canTerrainBePlaced(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()))
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
            if (canBuildingBePlaced(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()))
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
            if (canUnitBePlaced(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()))
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
        return pCurrentBuilding->canBuildingBePlaced(pMap->getTerrain(x, y)->getTerrainID());
    }
    return false;
}

bool EditorMenue::canUnitBePlaced(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
        QString movementType =m_EditorSelection->getCurrentSpUnit()->getMovementType();
        if (pMovementTableManager->getBaseMovementPoints(movementType, pMap->getTerrain(x, y)->getID()) > 0)
        {
            return true;
        }
    }
    return false;
}

void EditorMenue::placeTerrain(qint32 x, qint32 y)
{
    QSharedPointer<QVector<QPoint>> points;
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(0, 1);
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(0, 2);
            break;
        }
    }
    for (qint32 i = 0; i < points->size(); i++)
    {
        // nice we can place the terrain
        if (canTerrainBePlaced(x + points->at(i).x(), y + points->at(i).y()))
        {
            QString terrainID = m_EditorSelection->getCurrentTerrainID();
            GameMap* pMap = GameMap::getInstance();
            Mainapp* pApp = Mainapp::getInstance();
            QString function1 = "useTerrainAsBaseTerrain";
            QJSValueList args1;
            QJSValue useTerrainAsBaseTerrain = pApp->getInterpreter()->doFunction(terrainID, function1, args1);
            pMap->replaceTerrain(terrainID, x + points->at(i).x(), y + points->at(i).y(), useTerrainAsBaseTerrain.toBool(), true);
        }
    }
}

void EditorMenue::placeBuilding(qint32 x, qint32 y)
{
    QSharedPointer<QVector<QPoint>> points;
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(0, 1);
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(0, 2);
            break;
        }
    }
    for (qint32 i = 0; i < points->size(); i++)
    {
        // point still on the map great :)
        qint32 curX = x + points->at(i).x();
        qint32 curY = y + points->at(i).y();
        if (canBuildingBePlaced(curX, curY))
        {
            spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
            Building* pBuilding = new Building(pCurrentBuilding->getBuildingID());
            pBuilding->setOwner(pCurrentBuilding->getSpOwner());
            GameMap* pMap = GameMap::getInstance();
            pMap->getTerrain(curX, curY)->setBuilding(pBuilding);
        }
    }
}

void EditorMenue::placeUnit(qint32 x, qint32 y)
{
    QSharedPointer<QVector<QPoint>> points;
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(0, 1);
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(0, 2);
            break;
        }
    }
    for (qint32 i = 0; i < points->size(); i++)
    {
        // point still on the map great :)
        qint32 curX = x + points->at(i).x();
        qint32 curY = y + points->at(i).y();
        if (canUnitBePlaced(curX, curY))
        {
            spUnit pCurrentUnit = m_EditorSelection->getCurrentSpUnit();
            spUnit pUnit = new Unit(pCurrentUnit->getUnitID(), pCurrentUnit->getSpOwner());
            GameMap* pMap = GameMap::getInstance();
            pMap->getTerrain(curX, curY)->setUnit(pUnit);
        }
    }
}
