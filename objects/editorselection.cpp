#include "objects/editorselection.h"

#include "resource_management/objectmanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"

#include "coreengine/mainapp.h"

const float EditorSelection::xFactor = 1.5f;
const float EditorSelection::yFactor = 2.5f;

EditorSelection::EditorSelection()
    : QObject()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPosition(pMainapp->getSettings()->getWidth() - pMainapp->getSettings()->getWidth() / 4.0f, 0);
    m_BoxSelectionType = createV9Box(0, startHSelectionType, pMainapp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSize = createV9Box(0, startHPlacementSize, pMainapp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxSelectedPlayer = createV9Box(0, startHSelectedPlayer, pMainapp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSelection = createV9Box(0, startHTerrain, pMainapp->getSettings()->getWidth() / 4.0f, pMainapp->getSettings()->getHeight() - startHTerrain);
    createBoxPlacementSize();
    createBoxSelectionMode();

    m_CurrentSelector = new oxygine::Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelector->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
        m_CurrentSelector->addTween(tween);
    }
    else
    {
        m_CurrentSelector->setResAnim(pAnim);
    }
    m_BoxPlacementSelection->addChild(m_CurrentSelector);
    m_CurrentSelector->setPosition(frameSize, startH);
    m_CurrentSelector->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));

    // create terrains
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    // reserve vector size for fun and speed :D
    m_Terrains.reserve(pTerrainManager->getTerrainCount());
    for (qint32 i = 0; i < pTerrainManager->getTerrainCount(); i++)
    {
        m_Terrains.append(Terrain::createTerrain(pTerrainManager->getTerrainID(i), -10, -10));
        m_Terrains[i]->loadSprites();

        m_BoxPlacementSelection->addChild(m_Terrains[i]);
    }

    m_BoxPlacementSelection->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            emit sigClickedPlacementSelection(pTouchEvent->getPointer()->getPosition().x, pTouchEvent->getPointer()->getPosition().y);
        }
    });
    connect(this, SIGNAL(sigClickedPlacementSelection(qint32,qint32)), this, SLOT(ClickedPlacementSelection(qint32,qint32)), Qt::QueuedConnection);


    // load other sprites not shown in the starting screen
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
    {
        m_Buildings.append(new Building(pBuildingSpriteManager->getBuildingID(i), -10, -10));
        m_Buildings[i]->updateBuildingSprites();
        m_Buildings[i]->setVisible(false);
        m_BoxPlacementSelection->addChild(m_Buildings[i]);
    }


    // select terrains view
    updateTerrainView();
}

void EditorSelection::createBoxPlacementSize()
{
    qint32 yStartPos = 25;
    qint32 xChange = 40;
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_CurrentSelectorSize = new oxygine::Sprite();
    oxygine::ResAnim* pAnimMarker = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelectorSize->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    if (pAnimMarker->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimMarker), pAnimMarker->getTotalFrames() * GameMap::frameTime, -1);
        m_CurrentSelectorSize->addTween(tween);
    }
    else
    {
        m_CurrentSelectorSize->setResAnim(pAnimMarker);
    }
    m_BoxPlacementSize->addChild(m_CurrentSelectorSize);

    oxygine::spSprite pSpriteSmall = new oxygine::Sprite();

    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editor+small");
    pSpriteSmall->setResAnim(pAnim);
    pSpriteSmall->setPosition(frameSize, yStartPos);
    m_CurrentSelectorSize->setPosition(frameSize, yStartPos);
    m_BoxPlacementSize->addChild(pSpriteSmall);
    pSpriteSmall->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_SizeMode = PlacementSize::Small;
        m_CurrentSelectorSize->setPosition(frameSize, yStartPos);
    });
    // scale marker to correct size if needed
    m_CurrentSelectorSize->setScale(pAnim->getWidth() / pAnimMarker->getWidth());

    oxygine::spSprite pSpriteMedium = new oxygine::Sprite();
    pAnim = pObjectManager->getResAnim("editor+medium");
    pSpriteMedium->setResAnim(pAnim);
    pSpriteMedium->setPosition(frameSize + xChange, yStartPos);
    m_BoxPlacementSize->addChild(pSpriteMedium);
    pSpriteMedium->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_SizeMode = PlacementSize::Medium;
        m_CurrentSelectorSize->setPosition(frameSize + xChange, yStartPos);
    });

    oxygine::spSprite pSpriteBig = new oxygine::Sprite();
    pAnim = pObjectManager->getResAnim("editor+big");
    pSpriteBig->setResAnim(pAnim);
    pSpriteBig->setPosition(frameSize + xChange * 2, yStartPos );
    m_BoxPlacementSize->addChild(pSpriteBig);
    pSpriteBig->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_SizeMode = PlacementSize::Big;
        m_CurrentSelectorSize->setPosition(frameSize + xChange * 2, yStartPos);
    });
}

void EditorSelection::createPlayerSelection()
{
    // delete all
    // since we can get called when the player count changes :)
    m_BoxSelectedPlayer->removeChildren();
    m_Players.clear();

    updateSelectedPlayer(0);
}

void EditorSelection::updateSelectedPlayer(qint32 startIndex)
{
    m_playerStartIndex = startIndex;

}

void EditorSelection::createBoxSelectionMode()
{
    qint32 yStartPos = 25;
    qint32 xChange = 40;
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_CurrentSelectorMode = new oxygine::Sprite();
    oxygine::ResAnim* pAnimMarker = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelectorMode->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    if (pAnimMarker->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimMarker), pAnimMarker->getTotalFrames() * GameMap::frameTime, -1);
        m_CurrentSelectorMode->addTween(tween);
    }
    else
    {
        m_CurrentSelectorMode->setResAnim(pAnimMarker);
    }
    m_BoxSelectionType->addChild(m_CurrentSelectorMode);

    oxygine::spSprite pSpriteTerrainMode = new oxygine::Sprite();

    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editor+terrain");
    pSpriteTerrainMode->setResAnim(pAnim);
    pSpriteTerrainMode->setPosition(frameSize, yStartPos);
    m_CurrentSelectorMode->setPosition(frameSize, yStartPos);
    m_BoxSelectionType->addChild(pSpriteTerrainMode);
    pSpriteTerrainMode->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_Mode = EditorMode::Terrain;
        m_CurrentSelectorMode->setPosition(frameSize, yStartPos);
        updateTerrainView();
        ClickedPlacementSelection(this->getPosition().x + frameSize, startH + startHTerrain);
    });
    // scale marker to correct size if needed
    m_CurrentSelectorMode->setScale(pAnim->getWidth() / pAnimMarker->getWidth());

    oxygine::spSprite pSpriteBuildingMode = new oxygine::Sprite();

    pAnim = pObjectManager->getResAnim("editor+building");
    pSpriteBuildingMode->setResAnim(pAnim);
    pSpriteBuildingMode->setPosition(frameSize + xChange, yStartPos);
    m_BoxSelectionType->addChild(pSpriteBuildingMode);
    pSpriteBuildingMode->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_Mode = EditorMode::Building;
        m_CurrentSelectorMode->setPosition(frameSize + xChange, yStartPos);
        updateBuildingView();
        ClickedPlacementSelection(this->getPosition().x + frameSize, startH + startHTerrain);
    });

    oxygine::spSprite pSpriteUnitMode = new oxygine::Sprite();

    pAnim = pObjectManager->getResAnim("editor+unit");
    pSpriteUnitMode->setResAnim(pAnim);
    pSpriteUnitMode->setPosition(frameSize + xChange * 2, yStartPos);
    m_BoxSelectionType->addChild(pSpriteUnitMode);
    pSpriteUnitMode->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_Mode = EditorMode::Unit;
        m_CurrentSelectorMode->setPosition(frameSize + xChange * 2, yStartPos);
        // todo add update unit view
    });

}

oxygine::spSprite EditorSelection::createV9Box(qint32 x, qint32 y, qint32 width, qint32 heigth)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSprite = new oxygine::Box9Sprite();
    pSprite->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pSprite->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    this->addChild(pSprite);
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editorselection");
    pSprite->setResAnim(pAnim);
    pSprite->setSize(width, heigth);
    pSprite->setPosition(x, y);
    pSprite->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    return pSprite;
}

void EditorSelection::updateTerrainView()
{
    initSelection();
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    for (qint32 i = m_StartIndex; i < pTerrainManager->getTerrainCount(); i++)
    {
        m_Terrains[i]->setVisible(true);
    }
}

void EditorSelection::updateBuildingView()
{
    initSelection();
    for (qint32 i = m_StartIndex; i < m_Buildings.size(); i++)
    {
        m_Buildings[i]->setVisible(true);
    }
}

void EditorSelection::initSelection()
{
    qint32 posY = startH;
    qint32 xCounter = 0;

    for (qint32 i = 0; i < m_Buildings.size(); i++)
    {
        qint32 posX = frameSize + xCounter * GameMap::Imagesize * xFactor;
        if (posX > m_BoxPlacementSelection->getWidth() - GameMap::Imagesize - frameSize)
        {
            posY += GameMap::Imagesize * yFactor;
            xCounter = 0;
            posX = frameSize;
        }
        m_Buildings[i]->setPosition(posX, posY);
        m_Buildings[i]->setVisible(false);
        xCounter++;
    }
    posY = startH;
    xCounter = 0;
    for (qint32 i = 0; i < m_Terrains.size(); i++)
    {
        qint32 posX = frameSize + xCounter * GameMap::Imagesize * xFactor;
        if (posX > m_BoxPlacementSelection->getWidth() - GameMap::Imagesize - frameSize)
        {
            posY += GameMap::Imagesize * yFactor;
            xCounter = 0;
            posX = frameSize;
        }
        m_Terrains[i]->setPosition(posX, posY);
        m_Terrains[i]->setVisible(false);
        xCounter++;
    }
}

void EditorSelection::ClickedPlacementSelection(qint32 x, qint32 y)
{
    qint32 curX = x - this->getPosition().x - frameSize;
    qint32 curY = y - startH - startHTerrain;
    if ((curX >= 0) && (curY >= 0) && (curX < m_BoxPlacementSelection->getWidth() - GameMap::Imagesize - frameSize))
    {
        // check modulo fraction first
        qint32 xHit = curX % static_cast<qint32>(GameMap::Imagesize * xFactor);
        if (xHit <= GameMap::Imagesize)
        {
            xHit = curX / (GameMap::Imagesize * xFactor);
            qint32 yHit = curY % static_cast<qint32>(GameMap::Imagesize * yFactor);
            if (yHit <= GameMap::Imagesize)
            {
                qint32 xCount = (m_BoxPlacementSelection->getWidth() - GameMap::Imagesize - frameSize - frameSize) / (GameMap::Imagesize * xFactor) + 1;
                m_selectedIndex.setZ(xCount);
                yHit = curY / (GameMap::Imagesize * yFactor);
                // valid click?
                bool valid = false;
                switch (m_Mode)
                {
                    case EditorMode::Terrain:
                    {
                        if (xHit + yHit * xCount < m_Terrains.size())
                        {
                            valid = true;
                        }
                        break;
                    }
                    case EditorMode::Building:
                    {
                        if (xHit + yHit * xCount < m_Buildings.size())
                        {
                            valid = true;
                        }
                        break;
                    }
                }
                if (valid)
                {
                    m_selectedIndex.setX(xHit);
                    m_selectedIndex.setY(yHit);
                    m_CurrentSelector->setPosition(frameSize + xHit * GameMap::Imagesize * xFactor, startH + GameMap::Imagesize * yFactor * yHit);
                }
            }

        }
    }

}

void EditorSelection::selectTerrain(const QString& terrainID)
{
    for (qint32 i = 0; i < m_Terrains.size(); i++)
    {
        if (m_Terrains[i]->getTerrainID() == terrainID)
        {
            // calc position
            m_selectedIndex.setX(i % static_cast<qint32>(m_selectedIndex.z()));
            m_selectedIndex.setY(i / static_cast<qint32>(m_selectedIndex.z()));
            m_CurrentSelector->setPosition(frameSize + m_selectedIndex.x() * GameMap::Imagesize * xFactor, startH + GameMap::Imagesize * yFactor * m_selectedIndex.y());
        }
    }
}

EditorSelection::PlacementSize EditorSelection::getSizeMode() const
{
    return m_SizeMode;
}
