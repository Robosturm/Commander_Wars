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
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setWidth(pApp->getSettings()->getWidth() / 4.0f);
    this->setPosition(pApp->getSettings()->getWidth() - pApp->getSettings()->getWidth() / 4.0f, 0);
    m_BoxSelectionType = createV9Box(0, startHSelectionType, pApp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSize = createV9Box(0, startHPlacementSize, pApp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxSelectedPlayer = createV9Box(0, startHSelectedPlayer, pApp->getSettings()->getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSelection = createV9Box(0, startHTerrain, pApp->getSettings()->getWidth() / 4.0f, pApp->getSettings()->getHeight() - startHTerrain);
    m_PlacementSelectionSlider = new oxygine::SlidingActor();
    m_PlacementSelectionSlider->setPosition(10, 50);
    m_PlacementSelectionSlider->setSize(m_BoxPlacementSelection->getWidth() - 20,
                                        m_BoxPlacementSelection->getHeight() - 100);
    m_BoxPlacementSelection->addChild(m_PlacementSelectionSlider);
    m_PlacementActor = new oxygine::Actor();
    m_PlacementActor->setY(-GameMap::Imagesize);
    m_PlacementSelectionSlider->setContent(m_PlacementActor);

    qint32 xCount = (m_PlacementSelectionSlider->getWidth() - GameMap::Imagesize - frameSize - frameSize) / (GameMap::Imagesize * xFactor) + 1;
    m_selectedIndex.setZ(xCount);
    createBoxPlacementSize();
    createBoxSelectionMode();
    createPlayerSelection();

    m_CurrentSelector = new oxygine::Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelector->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
        m_CurrentSelector->addTween(tween);
    }
    else
    {
        m_CurrentSelector->setResAnim(pAnim);
    }

    oxygine::spButton pButtonTop = new oxygine::Button();
    pButtonTop->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    pButtonTop->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    oxygine::Sprite* ptr = pButtonTop.get();
    pButtonTop->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButtonTop->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButtonTop->setFlippedY(true);
    pButtonTop->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_PlacementActor->setY(m_PlacementActor->getY() - GameMap::Imagesize);
        if (m_PlacementActor->getHeight() < m_PlacementSelectionSlider->getHeight())
        {
            m_PlacementActor->setY(-GameMap::Imagesize);
        }
        else if (m_PlacementActor->getY() < -m_PlacementActor->getHeight() + m_PlacementSelectionSlider->getHeight())
        {
            m_PlacementActor->setY(-m_PlacementActor->getHeight() + m_PlacementSelectionSlider->getHeight());
        }
    });
    pButtonTop->setPosition(m_BoxPlacementSelection->getWidth() / 2 - pButtonTop->getWidth() / 2, 15);
    m_BoxPlacementSelection->addChild(pButtonTop);

    oxygine::spButton pButtonDown = new oxygine::Button();
    pButtonDown->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    pButtonDown->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    ptr = pButtonDown.get();
    pButtonDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButtonDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButtonDown->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_PlacementActor->setY(m_PlacementActor->getY() + GameMap::Imagesize);
        if (m_PlacementActor->getY() > -GameMap::Imagesize)
        {
            m_PlacementActor->setY(-GameMap::Imagesize);
        }
    });
    pButtonDown->setPosition(m_BoxPlacementSelection->getWidth() / 2 - pButtonTop->getWidth() / 2, m_BoxPlacementSelection->getHeight() - pButtonDown->getHeight() - 18);
    m_BoxPlacementSelection->addChild(pButtonDown);

    m_PlacementActor->addChild(m_CurrentSelector);
    m_CurrentSelector->setPosition(frameSize, startH);
    m_CurrentSelector->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));

    // create terrains
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    // reserve vector size for fun and speed :D
    m_Terrains.reserve(pTerrainManager->getTerrainCount());
    QVector<qint32> terrainGroups;
    for (qint32 i = 0; i < pTerrainManager->getTerrainCount(); i++)
    {
        qint32 terrainGroup = pTerrainManager->getTerrainGroup(i);
        if (!terrainGroups.contains(terrainGroup))
        {
            terrainGroups.append(terrainGroup);
        }
    }
    for (qint32 i2 = 0; i2 < terrainGroups.size(); i2++)
    {
        for (qint32 i = 0; i < pTerrainManager->getTerrainCount(); i++)
        {
            if (pTerrainManager->getTerrainGroup(i) == terrainGroups[i2])
            {
                m_Terrains.append(Terrain::createTerrain(pTerrainManager->getTerrainID(i), -10, -10, ""));
                m_Terrains[m_Terrains.size() - 1]->loadSprites();
                m_PlacementActor->addChild(m_Terrains[m_Terrains.size() - 1]);
            }
        }
    }

    connect(this, &EditorSelection::sigClickedPlacementSelection, this, &EditorSelection::ClickedPlacementSelection, Qt::QueuedConnection);
    connect(this, &EditorSelection::sigUpdateSelectedPlayer, this, &EditorSelection::updateSelectedPlayer);

    // load other sprites not shown in the starting screen
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
    {
        spBuilding building = new Building(pBuildingSpriteManager->getBuildingID(i));
        qint32 width = building->getBuildingWidth();
        qint32 heigth = building->getBuildingHeigth();
        building->setScaleX(1.0f / static_cast<float>(width));
        building->setScaleY(1.0f / static_cast<float>(heigth));
        m_Buildings.append(building);
        m_Buildings[i]->updateBuildingSprites(false);
        spTerrain pSprite = Terrain::createTerrain(building->getBaseTerrain()[0], -1, -1, "");
        pSprite->loadSprites();
        pSprite->setPriority(-100);
        pSprite->setScaleX(1 / building->getScaleX() * GameMap::Imagesize / pAnim->getWidth());
        pSprite->setScaleY(1 / building->getScaleY() * GameMap::Imagesize / pAnim->getHeight());
        if (width > 1)
        {
            pSprite->oxygine::Actor::setX(-GameMap::Imagesize * (width - 1));
        }
        if (heigth > 1)
        {
            pSprite->oxygine::Actor::setY(-GameMap::Imagesize * (heigth - 1));
        }
        m_Buildings[i]->addChild(pSprite);
        m_Buildings[i]->setVisible(false);
        m_PlacementActor->addChild(m_Buildings[i]);
    }

    // load other sprites not shown in the starting screen
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();

    spTerrain plains = Terrain::createTerrain("PLAINS", -1, -1, "");
    spTerrain sea = Terrain::createTerrain("SEA", -1, -1, "");

    QVector<GameEnums::UnitType> unitTypes;
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        GameEnums::UnitType unitType = pUnitSpriteManager->getUnitType(i);
        if (!unitTypes.contains(unitType))
        {
            unitTypes.append(unitType);
        }
    }
    for (qint32 i2 = 0; i2 < unitTypes.size(); i2++)
    {
        for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
        {
            if (pUnitSpriteManager->getUnitType(i) == unitTypes[i2])
            {
                spUnit unit = new Unit(pUnitSpriteManager->getUnitID(i), m_Players.at(1)->getOwner(), false);
                m_Units.append(unit);
                oxygine::spSprite pSprite = new oxygine::Sprite();
                QString movementType = unit->getMovementType();
                if (pMovementTableManager->getBaseMovementPoints(movementType, plains.get(), unit.get()) > 0)
                {
                    pAnim = pTerrainManager->getResAnim("plains+0");
                    pSprite->setResAnim(pAnim);
                }
                else if (pMovementTableManager->getBaseMovementPoints(movementType, sea.get(), unit.get()) > 0)
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
                pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
                unit->addChild(pSprite);
                unit->setVisible(false);
                m_PlacementActor->addChild(unit);
            }
        }
    }


    // select terrains view
    updateTerrainView();
}

void EditorSelection::createBoxPlacementSize()
{
    qint32 yStartPos = 20;
    qint32 xChange = 40;
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_CurrentSelectorSize = new oxygine::Sprite();
    oxygine::ResAnim* pAnimMarker = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelectorSize->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    if (pAnimMarker->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimMarker), oxygine::timeMS(pAnimMarker->getTotalFrames() * GameMap::frameTime), -1);
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
        emit sigSelectionChanged();
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
        emit sigSelectionChanged();
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
        emit sigSelectionChanged();
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
        emit sigSelectionChanged();
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
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButtonLeft->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
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
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });

    pButtonRight->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButtonRight->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_playerStartIndex += 1;
        if (m_playerStartIndex >= m_Players.size() - calcMaxPlayerSelection())
        {
            m_playerStartIndex = m_Players.size() - calcMaxPlayerSelection();
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
            pBuilding->setOwner(GameMap::getInstance()->getPlayer(i));
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
            changeSelectedPlayer(i);
        });
    }
    updateSelectedPlayer();
    changeSelectedPlayer(0);
}

void EditorSelection::changeSelectedPlayer(qint32 player)
{
    // update buildings
    for (qint32 i2 = 0; i2 < m_Buildings.size(); i2++)
    {
        if (player < 0)
        {
            m_Buildings.at(i2)->setOwner(nullptr);
        }
        else
        {
            m_Buildings.at(i2)->setOwner(m_Players.at(player + 1)->getOwner());
        }
    }
    // update units
    for (qint32 i2 = 0; i2 < m_Units.size(); i2++)
    {
        if (player < 0)
        {
            // do nothing :)
            m_Units.at(i2)->setOwner(m_Players.at(1)->getOwner());
        }
        else
        {
            m_Units.at(i2)->setOwner(m_Players.at(player + 1)->getOwner());
        }
    }
}

void EditorSelection::updateSelectedPlayer()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
    pApp->continueThread();
}

qint32 EditorSelection::calcMaxPlayerSelection()
{
    return (m_BoxSelectedPlayer->getScaledWidth() - 100) / frameSize;
}


void EditorSelection::createBoxSelectionMode()
{
    qint32 yStartPos = 20;
    qint32 xChange = 40;
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_CurrentSelectorMode = new oxygine::Sprite();
    oxygine::ResAnim* pAnimMarker = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelectorMode->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    if (pAnimMarker->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimMarker), oxygine::timeMS(pAnimMarker->getTotalFrames() * GameMap::frameTime), -1);
        m_CurrentSelectorMode->addTween(tween);
    }
    else
    {
        m_CurrentSelectorMode->setResAnim(pAnimMarker);
    }
    m_BoxSelectionType->addChild(m_CurrentSelectorMode);

    m_pSpriteTerrainMode = new oxygine::Sprite();

    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editor+terrain");
    m_pSpriteTerrainMode->setResAnim(pAnim);
    m_pSpriteTerrainMode->setPosition(frameSize, yStartPos);
    m_CurrentSelectorMode->setPosition(frameSize, yStartPos);
    m_BoxSelectionType->addChild(m_pSpriteTerrainMode);
    m_pSpriteTerrainMode->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_Mode = EditorMode::Terrain;
        m_CurrentSelectorMode->setPosition(frameSize, yStartPos);
        updateTerrainView();
        emit sigClickedPlacementSelection(0, 0);
    });
    // scale marker to correct size if needed
    m_CurrentSelectorMode->setScale(pAnim->getWidth() / pAnimMarker->getWidth());

    m_pSpriteBuildingMode = new oxygine::Sprite();

    pAnim = pObjectManager->getResAnim("editor+building");
    m_pSpriteBuildingMode->setResAnim(pAnim);
    m_pSpriteBuildingMode->setPosition(frameSize + xChange, yStartPos);
    m_BoxSelectionType->addChild(m_pSpriteBuildingMode);
    m_pSpriteBuildingMode->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_Mode = EditorMode::Building;
        m_CurrentSelectorMode->setPosition(frameSize + xChange, yStartPos);
        updateBuildingView();
        emit sigClickedPlacementSelection(0, 0);
    });

    m_pSpriteUnitMode = new oxygine::Sprite();

    pAnim = pObjectManager->getResAnim("editor+unit");
    m_pSpriteUnitMode->setResAnim(pAnim);
    m_pSpriteUnitMode->setPosition(frameSize + xChange * 2, yStartPos);
    m_BoxSelectionType->addChild(m_pSpriteUnitMode);
    m_pSpriteUnitMode->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        m_Mode = EditorMode::Unit;
        m_CurrentSelectorMode->setPosition(frameSize + xChange * 2, yStartPos);
        updateUnitView();
        emit sigClickedPlacementSelection(0, 0);
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
    m_PlacementActor->setHeight(m_Terrains[m_Terrains.size() - 1]->oxygine::Actor::getY() + GameMap::Imagesize + 5);
    m_PlacementSelectionSlider->snap();
    m_PlacementActor->setY(-GameMap::Imagesize);
}

void EditorSelection::updateBuildingView()
{
    initSelection();
    for (qint32 i = m_StartIndex; i < m_Buildings.size(); i++)
    {
        m_Buildings[i]->setVisible(true);
    }
    m_PlacementActor->setHeight(m_Buildings[m_Buildings.size() - 1]->oxygine::Actor::getY() + GameMap::Imagesize + 5);
    m_PlacementSelectionSlider->snap();
    m_PlacementActor->setY(-GameMap::Imagesize);
}

void EditorSelection::updateUnitView()
{
    initSelection();
    for (qint32 i = m_StartIndex; i < m_Units.size(); i++)
    {
        m_Units[i]->setVisible(true);
    }
    m_PlacementActor->setHeight(m_Units[m_Units.size() - 1]->oxygine::Actor::getY() + GameMap::Imagesize + 5);
    m_PlacementSelectionSlider->snap();
    m_PlacementActor->setY(-GameMap::Imagesize);
}

void EditorSelection::initSelection()
{
    qint32 posY = startH;
    qint32 xCounter = 0;
    qint32 yCounter = 0;
    for (qint32 i = 0; i < m_Buildings.size(); i++)
    {
        qint32 posX = frameSize + xCounter * GameMap::Imagesize * xFactor;
        if (xCounter >= m_selectedIndex.z())
        {
            posY += GameMap::Imagesize * yFactor;
            xCounter = 0;
            yCounter++;
            posX = frameSize;
        }
        qint32 width = m_Buildings[i]->getBuildingWidth();
        qint32 heigth = m_Buildings[i]->getBuildingHeigth();
        m_Buildings[i]->setX(posX + GameMap::Imagesize * (width - 1) / (width));
        m_Buildings[i]->setY(posY + GameMap::Imagesize * (heigth - 1) / (heigth));
        m_Buildings[i]->setVisible(false);
        m_Buildings[i]->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigClickedPlacementSelection(xCounter, yCounter);
        });
        xCounter++;
    }
    posY = startH;
    xCounter = 0;
    yCounter = 0;
    for (qint32 i = 0; i < m_Terrains.size(); i++)
    {
        qint32 posX = frameSize + xCounter * GameMap::Imagesize * xFactor;
        if (xCounter >= m_selectedIndex.z())
        {
            posY += GameMap::Imagesize * yFactor;
            xCounter = 0;
            yCounter++;
            posX = frameSize;
        }
        m_Terrains[i]->setPosition(posX, posY);
        m_Terrains[i]->setVisible(false);
        m_Terrains[i]->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigClickedPlacementSelection(xCounter, yCounter);
        });
        xCounter++;
    }
    posY = startH;
    xCounter = 0;
    yCounter = 0;
    for (qint32 i = 0; i < m_Units.size(); i++)
    {
        qint32 posX = frameSize + xCounter * GameMap::Imagesize * xFactor;
        if (xCounter >= m_selectedIndex.z())
        {
            posY += GameMap::Imagesize * yFactor;
            xCounter = 0;
            yCounter++;
            posX = frameSize;
        }
        m_Units[i]->setPosition(posX, posY);
        m_Units[i]->setVisible(false);
        m_Units[i]->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigClickedPlacementSelection(xCounter, yCounter);
        });
        xCounter++;
    }
}

void EditorSelection::ClickedPlacementSelection(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_selectedIndex.setX(x);
    m_selectedIndex.setY(y);
    m_CurrentSelector->setPosition(frameSize + x * GameMap::Imagesize * xFactor, startH + GameMap::Imagesize * yFactor * y);
    emit sigSelectionChanged();
    pApp->continueThread();
}

void EditorSelection::selectTerrain(QString terrainID)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Mode = EditorMode::Terrain;
    m_CurrentSelectorMode->setPosition(m_pSpriteTerrainMode->getPosition());
    updateTerrainView();
    for (qint32 i = 0; i < m_Terrains.size(); i++)
    {
        if (m_Terrains[i]->getTerrainID() == terrainID)
        {
            if (m_selectedIndex.z() > 0)
            {
                // calc position
                m_selectedIndex.setX(i % static_cast<qint32>(m_selectedIndex.z()));
                m_selectedIndex.setY(i / static_cast<qint32>(m_selectedIndex.z()));
                m_CurrentSelector->setPosition(frameSize + m_selectedIndex.x() * GameMap::Imagesize * xFactor, startH + GameMap::Imagesize * yFactor * m_selectedIndex.y());
            }
        }
    }
     pApp->continueThread();
}

void EditorSelection::selectBuilding(QString buildingID)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Mode = EditorMode::Building;
    m_CurrentSelectorMode->setPosition(m_pSpriteBuildingMode->getPosition());
    updateBuildingView();
    for (qint32 i = 0; i < m_Buildings.size(); i++)
    {
        if (m_Buildings[i]->getBuildingID() == buildingID)
        {
            if (m_selectedIndex.z() > 0)
            {
                // calc position
                m_selectedIndex.setX(i % static_cast<qint32>(m_selectedIndex.z()));
                m_selectedIndex.setY(i / static_cast<qint32>(m_selectedIndex.z()));
                m_CurrentSelector->setPosition(frameSize + m_selectedIndex.x() * GameMap::Imagesize * xFactor, startH + GameMap::Imagesize * yFactor * m_selectedIndex.y());
                emit sigSelectionChanged();
            }
        }
    }
     pApp->continueThread();
}

void EditorSelection::selectUnit(QString unitID)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Mode = EditorMode::Unit;
    m_CurrentSelectorMode->setPosition(m_pSpriteUnitMode->getPosition());
    updateUnitView();
    for (qint32 i = 0; i < m_Units.size(); i++)
    {
        if (m_Units[i]->getUnitID() == unitID)
        {
            if (m_selectedIndex.z() > 0)
            {
                // calc position
                m_selectedIndex.setX(i % static_cast<qint32>(m_selectedIndex.z()));
                m_selectedIndex.setY(i / static_cast<qint32>(m_selectedIndex.z()));
                m_CurrentSelector->setPosition(frameSize + m_selectedIndex.x() * GameMap::Imagesize * xFactor, startH + GameMap::Imagesize * yFactor * m_selectedIndex.y());
            }
        }
    }
     pApp->continueThread();
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
