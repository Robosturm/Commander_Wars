#include "menue/editormenue.h"

#include "coreengine/mainapp.h"

EditorMenue::EditorMenue()
{
    this->addChild(m_EditorSelection);
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        emit sigOnMapClicked();
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->getPointer()->isPressed())
            {
                emit sigOnMapClicked();
            }
        }
    });

    // connecting stuff
    connect(this, SIGNAL(sigOnMapClicked()), this, SLOT(onMapClicked()), Qt::QueuedConnection);
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

void EditorMenue::onMapClicked()
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function = "canBePlaced";
    QJSValueList args;
    args << QJSValue(x);
    args << QJSValue(y);
    QJSValue placeable = pApp->getInterpreter()->doFunction(terrainID, function, args);
    if (placeable.isBool() && placeable.toBool())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EditorMenue::placeTerrain(qint32 x, qint32 y)
{
    QString terrainID = m_EditorSelection->getCurrentTerrainID();
    Mainapp* pApp = Mainapp::getInstance();
    QString function = "canBePlaced";
    QJSValueList args;
    args << QJSValue(x);
    args << QJSValue(y);
    QJSValue placeable = pApp->getInterpreter()->doFunction(terrainID, function, args);
    if (placeable.isBool() && placeable.toBool())
    {
        // nice we can place the terrain
        GameMap* pMap = GameMap::getInstance();
        QString function1 = "useTerrainAsBaseTerrain";
        QJSValueList args1;
        QJSValue useTerrainAsBaseTerrain = pApp->getInterpreter()->doFunction(terrainID, function1, args1);
        pMap->replaceTerrain(terrainID, x, y, useTerrainAsBaseTerrain.toBool());
    }
}
