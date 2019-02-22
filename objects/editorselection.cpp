#include "objects/editorselection.h"

#include "resource_management/objectmanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/movementtablemanager.h"

#include "coreengine/mainapp.h"

#include "game/co.h"

const float EditorSelection::xFactor = 1.5f;
const float EditorSelection::yFactor = 2.5f;

EditorSelection::EditorSelection()
    : QObject()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setWidth(pMainapp->getSettings()->getWidth() / 4.0f);
    this->setPosition(pMainapp->getSettings()->getWidth() - pMainapp->getSettings()->getWidth() / 4.0f, 0);
    m_BoxSelectionType = createV9Box(0, startHSelectionType, pMainapp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSize = createV9Box(0, startHPlacementSize, pMainapp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxSelectedPlayer = createV9Box(0, startHSelectedPlayer, pMainapp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSelection = createV9Box(0, startHTerrain, pMainapp->getSettings()->getWidth() / 4.0f, pMainapp->getSettings()->getHeight() - startHTerrain);
    createBoxPlacementSize();
    createBoxSelectionMode();
    createPlayerSelection();

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
    connect(this, SIGNAL(sigUpdateSelectedPlayer()), this, SLOT(updateSelectedPlayer()));

    // load other sprites not shown in the starting screen
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
    {
        spBuilding building = new Building(pBuildingSpriteManager->getBuildingID(i));
        m_Buildings.append(building);
        m_Buildings[i]->updateBuildingSprites();
        oxygine::spSprite pSprite = new oxygine::Sprite();
        pAnim = pTerrainManager->getResAnim("plains+0");
        pSprite->setResAnim(pAnim);
        pSprite->setPriority(-100);
        m_Buildings[i]->addChild(pSprite);
        m_Buildings[i]->setVisible(false);
        m_BoxPlacementSelection->addChild(m_Buildings[i]);
    }

    // load other sprites not shown in the starting screen
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();

    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        spUnit unit = new Unit(pUnitSpriteManager->getUnitID(i), m_Players.at(1)->getSpOwner());
        m_Units.append(unit);
        oxygine::spSprite pSprite = new oxygine::Sprite();
        QString movementType = m_Units.at(i)->getMovementType();
        if (pMovementTableManager->getBaseMovementPoints(movementType, "PLAINS") > 0)
        {
            pAnim = pTerrainManager->getResAnim("plains+0");
            pSprite->setResAnim(pAnim);
        }
        else if (pMovementTableManager->getBaseMovementPoints(movementType, "SEA") > 0)
        {
            pAnim = pTerrainManager->getResAnim("SEA");
            pSprite->setResAnim(pAnim);
        }
        else
        {
            // todo maybe to something about this here
            pAnim = pTerrainManager->getResAnim("plains+0");
            pSprite->setResAnim(pAnim);
        }
        pSprite->setPriority(-100);
        m_Units[i]->addChild(pSprite);
        m_Units[i]->setVisible(false);
        m_BoxPlacementSelection->addChild(m_Units[i]);
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

    oxygine::spSprite pSpriteFill = new oxygine::Sprite();
    pAnim = pObjectManager->getResAnim("editor+fill");
    pSpriteFill->setResAnim(pAnim);
    pSpriteFill->setPosition(frameSize + xChange * 3, yStartPos );
    m_BoxPlacementSize->addChild(pSpriteFill);
    pSpriteFill->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_SizeMode = PlacementSize::Fill;
        m_CurrentSelectorSize->setPosition(frameSize + xChange * 3, yStartPos);
    });
}

void EditorSelection::createPlayerSelection()
{
    // delete all
    // since we can get called when the player count changes :)
    m_BoxSelectedPlayer->removeChildren();
    m_Players.clear();

    oxygine::spButton pButtonLeft = new oxygine::Button();
    pButtonLeft->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButtonLeft->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    oxygine::Sprite* ptr = pButtonLeft.get();
    pButtonLeft->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });

    pButtonLeft->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    pButtonLeft->setFlippedX(true);
    pButtonLeft->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_playerStartIndex -= 1;
        if (m_playerStartIndex < 0)
        {
            m_playerStartIndex = 0;
        }
        emit sigUpdateSelectedPlayer();
    });
    pButtonLeft->setPosition(20, 20);
    m_BoxSelectedPlayer->addChild(pButtonLeft);

    oxygine::spButton pButtonRight = new oxygine::Button();
    pButtonRight->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButtonRight->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    ptr = pButtonRight.get();
    pButtonRight->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });

    pButtonRight->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
    });
    pButtonRight->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_playerStartIndex += 1;
        if (m_playerStartIndex >= m_Players.size() - calcMaxPlayerSelection())
        {
            m_playerStartIndex = m_Players.size() - 1 - calcMaxPlayerSelection();
            if (m_playerStartIndex < 0)
            {
                m_playerStartIndex = 0;
            }
        }
        emit sigUpdateSelectedPlayer();
    });
    pButtonRight->setPosition(m_BoxSelectedPlayer->getScaledWidth() - 40, 20);
    m_BoxSelectedPlayer->addChild(pButtonRight);
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    for (qint32 i = -1; i < GameMap::getInstance()->getPlayerCount(); i++)
    {
        spBuilding pBuilding = new Building("HQ");
        oxygine::spSprite pSprite = new oxygine::Sprite();
        oxygine::ResAnim* pAnim = pTerrainManager->getResAnim("plains+0");
        pSprite->setResAnim(pAnim);
        pSprite->setPriority(-100);
        pBuilding->addChild(pSprite);
        m_Players.append(pBuilding);
        if (i >= 0)
        {
            pBuilding->setOwner(GameMap::getInstance()->getspPlayer(i));
        }
        else
        {
            pBuilding->setOwner(nullptr);
        }
        m_BoxSelectedPlayer->addChild(pBuilding);
        pBuilding->setVisible(true);
        pBuilding->setPosition(25 * i, 32);
        pBuilding->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            // update buildings
            for (qint32 i2 = 0; i2 < m_Buildings.size(); i2++)
            {
                if (i < 0)
                {
                    m_Buildings.at(i2)->setOwner(nullptr);
                }
                else
                {
                    m_Buildings.at(i2)->setOwner(m_Players.at(i + 1)->getSpOwner());
                }
            }
            // update units
            for (qint32 i2 = 0; i2 < m_Units.size(); i2++)
            {
                if (i < 0)
                {
                    // do nothing :)
                }
                else
                {
                    m_Units.at(i2)->setOwner(m_Players.at(i + 1)->getSpOwner());
                }
            }

        });
    }

    updateSelectedPlayer();
}

void EditorSelection::updateSelectedPlayer()
{
    for (qint32 i = 0; i < m_Players.size(); i++)
    {
        m_Players[i]->setVisible(false);
    }
    qint32 m_MaxPlayer = calcMaxPlayerSelection();
    for (qint32 i = m_playerStartIndex; i < m_playerStartIndex + m_MaxPlayer; i++)
    {
        if (i < m_Players.size())
        {
            m_Players[i]->setVisible(true);
            m_Players[i]->setPosition(50 + frameSize * (i - m_playerStartIndex), 32);
        }
    }
}

qint32 EditorSelection::calcMaxPlayerSelection()
{
    return (m_BoxSelectedPlayer->getScaledWidth() - 100) / frameSize;
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
        updateUnitView();
        ClickedPlacementSelection(this->getPosition().x + frameSize, startH + startHTerrain);
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

void EditorSelection::updateUnitView()
{
    initSelection();
    for (qint32 i = m_StartIndex; i < m_Units.size(); i++)
    {
        m_Units[i]->setVisible(true);
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
    posY = startH;
    xCounter = 0;
    for (qint32 i = 0; i < m_Units.size(); i++)
    {
        qint32 posX = frameSize + xCounter * GameMap::Imagesize * xFactor;
        if (posX > m_BoxPlacementSelection->getWidth() - GameMap::Imagesize - frameSize)
        {
            posY += GameMap::Imagesize * yFactor;
            xCounter = 0;
            posX = frameSize;
        }
        m_Units[i]->setPosition(posX, posY);
        m_Units[i]->setVisible(false);
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
                    case EditorMode::Unit:
                    {
                        if (xHit + yHit * xCount < m_Units.size())
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

spBuilding EditorSelection::getCurrentSpBuilding()
{
    return  m_Buildings.at(m_selectedIndex.x() + m_selectedIndex.y() * m_selectedIndex.z());
}

spUnit EditorSelection::getCurrentSpUnit()
{
    return  m_Units.at(m_selectedIndex.x() + m_selectedIndex.y() * m_selectedIndex.z());
}
