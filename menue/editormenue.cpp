#include "menue/editormenue.h"

#include "coreengine/mainapp.h"

#include "coreengine/pathfindingsystem.h"

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
