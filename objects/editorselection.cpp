#include "objects/editorselection.h"

#include "resource_management/objectmanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "game/co.h"

#include "objects/base/label.h"

const float EditorSelection::xFactor = 1.5f;
const float EditorSelection::yFactor = 2.5f;

EditorSelection::EditorSelection()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setWidth(Settings::getWidth() / 4.0f);
    this->setPosition(Settings::getWidth() - Settings::getWidth() / 4.0f, 0);
    m_BoxSelectionType = createV9Box(0, startHSelectionType, Settings::getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSize = createV9Box(0, startHPlacementSize, Settings::getWidth() / 4.0f, selectionHeight);
    m_BoxSelectedPlayer = createV9Box(0, startHSelectedPlayer, Settings::getWidth() / 4.0f, selectionHeight);
    m_BoxPlacementSelection = createV9Box(0, startHTerrain, Settings::getWidth() / 4.0f, Settings::getHeight() - startHTerrain);
    m_PlacementSelectionClip = new oxygine::ClipRectActor();
    m_PlacementSelectionClip->setPosition(10, 50);
    m_PlacementSelectionClip->setSize(m_BoxPlacementSelection->getWidth() - 20,
                                        m_BoxPlacementSelection->getHeight() - 100);
    m_BoxPlacementSelection->addChild(m_PlacementSelectionClip);
    m_PlacementActor = new oxygine::Actor();
    m_PlacementActor->setY(-GameMap::getImageSize());
    m_PlacementSelectionClip->addChild(m_PlacementActor);

    m_labelWidth = m_PlacementSelectionClip->getWidth() - GameMap::getImageSize() - frameSize - frameSize;
    m_xCount = m_labelWidth / (GameMap::getImageSize() * xFactor) + 1;
    createBoxPlacementSize();
    createBoxSelectionMode();
    createPlayerSelection();

    m_CurrentSelector = new oxygine::Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelector->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_CurrentSelector->setScale(GameMap::getImageSize() / pAnim->getWidth());
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime * 2), -1);
        m_CurrentSelector->addTween(tween);
    }
    else
    {
        m_CurrentSelector->setResAnim(pAnim);
    }

    oxygine::spButton pButtonTop = new oxygine::Button();
    pButtonTop->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    pButtonTop->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
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
        emit sigChangeScrollValue(1);
    });
    pButtonTop->setPosition(m_BoxPlacementSelection->getWidth() / 2 - pButtonTop->getWidth() / 2, 15);
    m_BoxPlacementSelection->addChild(pButtonTop);

    oxygine::spButton pButtonDown = new oxygine::Button();
    pButtonDown->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    pButtonDown->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
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
        emit sigChangeScrollValue(-1);
    });
    m_BoxPlacementSelection->addEventListener(oxygine::TouchEvent::WHEEL_DIR, [ = ](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
           emit sigChangeScrollValue(pTouchEvent->wheelDirection.y);
           pTouchEvent->stopPropagation();
        }
    });
    pButtonDown->setPosition(m_BoxPlacementSelection->getWidth() / 2 - pButtonTop->getWidth() / 2, m_BoxPlacementSelection->getHeight() - pButtonDown->getHeight() - 18);
    m_BoxPlacementSelection->addChild(pButtonDown);

    m_PlacementActor->addChild(m_CurrentSelector);
    m_CurrentSelector->setPosition(frameSize, startH);
    m_CurrentSelector->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    // create terrains
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    // reserve vector size for fun and speed :D
    m_Terrains.reserve(pTerrainManager->getCount());
    QStringList sortedTerrainIDs = pTerrainManager->getTerrainsSorted();
    for (const auto& terrainId : sortedTerrainIDs)
    {
        spTerrain pTerrain = Terrain::createTerrain(terrainId, -10, -10, "");
        pTerrain->setTooltipText(pTerrain->getTerrainName());
        m_Terrains.append(pTerrain);
        m_Terrains[m_Terrains.size() - 1]->loadSprites();
        m_PlacementActor->addChild(m_Terrains[m_Terrains.size() - 1]);
    }

    connect(this, &EditorSelection::sigUpdateSelectedPlayer, this, &EditorSelection::updateSelectedPlayer, Qt::QueuedConnection);
    connect(this, &EditorSelection::sigChangeSelectedPlayer, this, &EditorSelection::changeSelectedPlayer, Qt::QueuedConnection);
    connect(this, &EditorSelection::sigUpdateUnitView, this, &EditorSelection::updateUnitView, Qt::QueuedConnection);
    connect(this, &EditorSelection::sigUpdateTerrainView, this, &EditorSelection::updateTerrainView, Qt::QueuedConnection);
    connect(this, &EditorSelection::sigUpdateBuildingView, this, &EditorSelection::updateBuildingView, Qt::QueuedConnection);
    connect(this, &EditorSelection::sigChangeScrollValue, this, &EditorSelection::changeScrollValue, Qt::QueuedConnection);

    // load other sprites not shown in the starting screen
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        spBuilding building = new Building(pBuildingSpriteManager->getID(i));
        building->setTooltipText(building->getName());
        qint32 width = building->getBuildingWidth();
        qint32 heigth = building->getBuildingHeigth();
        building->setScaleX(1.0f / static_cast<float>(width));
        building->setScaleY(1.0f / static_cast<float>(heigth));
        m_Buildings.append(building);
        m_Buildings[i]->updateBuildingSprites(false);
        spTerrain pSprite = Terrain::createTerrain(building->getBaseTerrain()[0], -1, -1, "");
        pSprite->loadSprites();
        pSprite->setPriority(-100);
        pSprite->setScaleX(1 / building->getScaleX() * GameMap::getImageSize() / pAnim->getWidth());
        pSprite->setScaleY(1 / building->getScaleY() * GameMap::getImageSize() / pAnim->getHeight());
        if (width > 1)
        {
            pSprite->oxygine::Actor::setX(-GameMap::getImageSize() * (width - 1));
        }
        if (heigth > 1)
        {
            pSprite->oxygine::Actor::setY(-GameMap::getImageSize() * (heigth - 1));
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

    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    for (const auto& unitId : sortedUnits)
    {
        spUnit unit = new Unit(unitId, m_Players.at(1)->getOwner(), false);
        unit->setTooltipText(unit->getName());
        m_Units.append(unit);
        oxygine::spSprite pSprite = new oxygine::Sprite();
        QString movementType = unit->getMovementType();
        if (pMovementTableManager->getBaseMovementPoints(movementType, plains.get(), plains.get(), unit.get()) > 0)
        {
            pAnim = pTerrainManager->getResAnim("plains+0");
            pSprite->setResAnim(pAnim);
        }
        else if (pMovementTableManager->getBaseMovementPoints(movementType, sea.get(), sea.get(), unit.get()) > 0)
        {
            pAnim = pTerrainManager->getResAnim("SEA");
            pSprite->setResAnim(pAnim);
        }
        else
        {
            pAnim = pTerrainManager->getResAnim("plains+0");
            pSprite->setResAnim(pAnim);
        }
        pSprite->setPriority(-100);
        pSprite->setScale(GameMap::getImageSize() / pAnim->getWidth());
        unit->addChild(pSprite);
        unit->setVisible(false);
        m_PlacementActor->addChild(unit);
    }

    initSelection();
    // select terrains view
    updateTerrainView();
}

void EditorSelection::changeScrollValue(qint32 dir)
{
    if (dir > 0)
    {
        m_PlacementActor->setY(m_PlacementActor->getY() + GameMap::getImageSize());
        if (m_PlacementActor->getY() > -GameMap::getImageSize())
        {
            m_PlacementActor->setY(-GameMap::getImageSize());
        }
    }
    else
    {
        m_PlacementActor->setY(m_PlacementActor->getY() - GameMap::getImageSize());
        if (m_PlacementActor->getHeight() < m_PlacementSelectionClip->getHeight())
        {
            m_PlacementActor->setY(-GameMap::getImageSize());
        }
        else if (m_PlacementActor->getY() < -m_PlacementActor->getHeight() + m_PlacementSelectionClip->getHeight())
        {
            m_PlacementActor->setY(-m_PlacementActor->getHeight() + m_PlacementSelectionClip->getHeight());
        }
    }
}

void EditorSelection::createBoxPlacementSize()
{
    qint32 yStartPos = 20;
    qint32 xChange = 40;
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_CurrentSelectorSize = new oxygine::Sprite();
    oxygine::ResAnim* pAnimMarker = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelectorSize->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    if (pAnimMarker->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimMarker), oxygine::timeMS(pAnimMarker->getTotalFrames() * GameMap::frameTime * 2), -1);
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
    // since we can get called when the player count changes :)
    m_BoxSelectedPlayer->removeChildren();
    m_Players.clear();

    oxygine::spButton pButtonLeft = new oxygine::Button();
    pButtonLeft->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButtonLeft->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
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
    pButtonRight->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
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
            emit sigChangeSelectedPlayer(i);
        });
    }
    updateSelectedPlayer();
    changeSelectedPlayer(0);
}

void EditorSelection::changeSelectedPlayer(qint32 player)
{
    
    // update buildings
    if (player < 0)
    {
        m_currentPlayer = nullptr;
    }
    else
    {
        m_currentPlayer = m_Players.at(player + 1)->getOwner();
    }
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
    qint32 yStartPos = 20;
    qint32 xChange = 40;
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_CurrentSelectorMode = new oxygine::Sprite();
    oxygine::ResAnim* pAnimMarker = pObjectManager->getResAnim("editor+selector");
    m_CurrentSelectorMode->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    if (pAnimMarker->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnimMarker), oxygine::timeMS(pAnimMarker->getTotalFrames() * GameMap::frameTime * 2), -1);
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
        emit sigUpdateTerrainView();
        selectTerrain(0);
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
        emit sigUpdateBuildingView();
        selectBuilding(0);
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
        emit sigUpdateUnitView();
        selectUnit(0);
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
    pSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    return pSprite;
}

void EditorSelection::updateTerrainView()
{
    
    hideSelection();
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    for (qint32 i = m_StartIndex; i < pTerrainManager->getCount(); i++)
    {
        m_Terrains[i]->setVisible(true);
    }
    for (auto & item : m_terrainActors)
    {
        item->setVisible(true);
    }
    m_PlacementActor->setHeight(m_Terrains[m_Terrains.size() - 1]->oxygine::Actor::getY() + GameMap::getImageSize() + 5);
    m_PlacementActor->setY(-GameMap::getImageSize());
    selectTerrain(0);
    
}

void EditorSelection::updateBuildingView()
{
    
    hideSelection();
    for (qint32 i = m_StartIndex; i < m_Buildings.size(); i++)
    {
        m_Buildings[i]->setVisible(true);
    }
    m_PlacementActor->setHeight(m_Buildings[m_Buildings.size() - 1]->oxygine::Actor::getY() + GameMap::getImageSize() + 5);
    m_PlacementActor->setY(-GameMap::getImageSize());
    selectBuilding(0);
    
}

void EditorSelection::updateUnitView()
{
    
    hideSelection();
    for (qint32 i = m_StartIndex; i < m_Units.size(); i++)
    {
        m_Units[i]->setVisible(true);
    }
    for (auto & item : m_unitActors)
    {
        item->setVisible(true);
    }
    m_PlacementActor->setHeight(m_Units[m_Units.size() - 1]->oxygine::Actor::getY() + GameMap::getImageSize() + 5);
    m_PlacementActor->setY(-GameMap::getImageSize());
    selectUnit(0);
    
}

void EditorSelection::hideSelection()
{
    for (auto & item : m_Terrains)
    {
        item->setVisible(false);
    }
    for (auto & item : m_terrainActors)
    {
        item->setVisible(false);
    }
    for (auto & item : m_Buildings)
    {
        item->setVisible(false);
    }
    for (auto & item : m_Units)
    {
        item->setVisible(false);
    }
    for (auto & item : m_unitActors)
    {
        item->setVisible(false);
    }
}

void EditorSelection::initSelection()
{
    initBuildingSection();
    initTerrainSection();
    initUnitSelection();
}

void EditorSelection::initBuildingSection()
{
    qint32 posY = startH;
    qint32 xCounter = 0;
    for (qint32 i = 0; i < m_Buildings.size(); i++)
    {
        qint32 posX = getPosX(xCounter);
        if (xCounter >= m_xCount)
        {
            posY += GameMap::getImageSize() * yFactor;
            xCounter = 0;
            posX = frameSize;
        }
        qint32 width = m_Buildings[i]->getBuildingWidth();
        qint32 heigth = m_Buildings[i]->getBuildingHeigth();
        m_Buildings[i]->setX(posX + GameMap::getImageSize() * (width - 1) / (width));
        m_Buildings[i]->setY(posY + GameMap::getImageSize() * (heigth - 1) / (heigth));
        m_Buildings[i]->setVisible(false);
        m_Buildings[i]->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            selectBuilding(i);
        });
        xCounter++;
    }
}

void EditorSelection::initTerrainSection()
{
    qint32 posY = startH - GameMap::getImageSize();
    qint32 xCounter = 0;
    qint32 currentIdentifier = std::numeric_limits<qint32>::min();
    for (qint32 i = 0; i < m_Terrains.size(); i++)
    {
        createTerrainSectionLabel(i, currentIdentifier, xCounter, posY);
        qint32 posX = getPosX(xCounter);
        if (xCounter >= m_xCount)
        {
            posY += GameMap::getImageSize() * yFactor;
            xCounter = 0;
            posX = frameSize;
        }
        m_Terrains[i]->setPosition(posX, posY);
        m_Terrains[i]->setVisible(false);
        m_Terrains[i]->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            selectTerrain(i);
        });
        xCounter++;
    }
}

void EditorSelection::createTerrainSectionLabel(qint32 item, qint32 & currentIdentifier, qint32 & xCounter, qint32 & posY)
{
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    qint32 newIdentifier = m_Terrains[item]->getTerrainGroup();
    if (newIdentifier != currentIdentifier)
    {
        posY += GameMap::getImageSize();
        currentIdentifier = newIdentifier;
        xCounter = 0;
        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;

        spLabel pTextfield = new Label(m_labelWidth);
        pTextfield->setStyle(style);
        pTextfield->setPosition(getPosX(xCounter), posY);
        pTextfield->setHtmlText(pTerrainManager->getTerrainGroupName(currentIdentifier));
        pTextfield->setVisible(false);
        m_PlacementActor->addChild(pTextfield);
        m_terrainActors.append(pTextfield);
        posY += GameMap::getImageSize() * yFactor;
    }
}

void EditorSelection::initUnitSelection()
{
    qint32 posY = startH - GameMap::getImageSize();
    qint32 xCounter = 0;
    qint32 currentIdentifier = std::numeric_limits<qint32>::min();
    for (qint32 i = 0; i < m_Units.size(); i++)
    {
        createUnitSectionLabel(i, currentIdentifier, xCounter, posY);
        qint32 posX = getPosX(xCounter);
        if (xCounter >= m_xCount)
        {
            posY += GameMap::getImageSize() * 1.5f;
            xCounter = 0;
            posX = frameSize;
        }
        m_Units[i]->setPosition(posX, posY);
        m_Units[i]->setVisible(false);
        m_Units[i]->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            selectUnit(i);
        });
        xCounter++;
    }
}

void EditorSelection::createUnitSectionLabel(qint32 item, qint32 & currentIdentifier, qint32 & xCounter, qint32 & posY)
{
    qint32 newIdentifier = m_Units[item]->getUnitType();
    if (newIdentifier != currentIdentifier)
    {
        posY += GameMap::getImageSize();
        currentIdentifier = newIdentifier;
        xCounter = 0;
        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;

        spLabel pTextfield = new Label(m_labelWidth);
        pTextfield->setStyle(style);
        pTextfield->setPosition(getPosX(xCounter), posY);
        pTextfield->setHtmlText(UnitSpriteManager::getUnitTypeText(currentIdentifier));
        pTextfield->setVisible(false);
        m_PlacementActor->addChild(pTextfield);
        m_unitActors.append(pTextfield);
        posY += GameMap::getImageSize() * 1.5f;
    }
}

qint32 EditorSelection::getPosX(qint32 xCounter)
{
    return frameSize + xCounter * GameMap::getImageSize() * xFactor;
}

void EditorSelection::selectBuilding(qint32 building)
{
    m_selectedItem = building;
    qint32 width = m_Buildings[building]->getBuildingWidth();
    qint32 heigth = m_Buildings[building]->getBuildingHeigth();
    qint32 x = GameMap::getImageSize() * (width - 1) / (width);
    qint32 y = GameMap::getImageSize() * (heigth - 1) / (heigth);
    m_CurrentSelector->setPosition(m_Buildings[building]->oxygine::Actor::getPosition() - oxygine::Vector2(x, y));
    emit sigSelectionChanged();
}

void EditorSelection::selectUnit(qint32 unit)
{
    m_selectedItem = unit;
    m_CurrentSelector->setPosition(m_Units[unit]->oxygine::Actor::getPosition());
    emit sigSelectionChanged();
}

void EditorSelection::selectTerrain(qint32 terrain)
{
    m_selectedItem = terrain;
    m_CurrentSelector->setPosition(m_Terrains[terrain]->oxygine::Actor::getPosition());
    emit sigSelectionChanged();
}

void EditorSelection::selectTerrain(QString terrainID)
{
    
    m_Mode = EditorMode::Terrain;
    m_CurrentSelectorMode->setPosition(m_pSpriteTerrainMode->getPosition());
    updateTerrainView();
    for (qint32 i = 0; i < m_Terrains.size(); i++)
    {
        if (m_Terrains[i]->getTerrainID() == terrainID)
        {
            selectTerrain(i);
        }
    }
    
}

void EditorSelection::selectBuilding(QString buildingID)
{
    
    m_Mode = EditorMode::Building;
    m_CurrentSelectorMode->setPosition(m_pSpriteBuildingMode->getPosition());
    updateBuildingView();
    for (qint32 i = 0; i < m_Buildings.size(); i++)
    {
        if (m_Buildings[i]->getBuildingID() == buildingID)
        {
            selectBuilding(i);
        }
    }
    
}

void EditorSelection::selectUnit(QString unitID)
{
    
    m_Mode = EditorMode::Unit;
    m_CurrentSelectorMode->setPosition(m_pSpriteUnitMode->getPosition());
    updateUnitView();
    for (qint32 i = 0; i < m_Units.size(); i++)
    {
        if (m_Units[i]->getUnitID() == unitID)
        {
            selectUnit(i);
        }
    }
    
}

void EditorSelection::KeyInput(Qt::Key cur)
{
    if (cur == Settings::getKey_EditorPlaceTerrain())
    {
        oxygine::TouchEvent event(oxygine::TouchEvent::CLICK, false);
        event.target = m_pSpriteTerrainMode;
        event.currentTarget = m_pSpriteTerrainMode;
        m_pSpriteTerrainMode->dispatchEvent(&event);
    }
    else if (cur  == Settings::getKey_EditorPlaceBuilding())
    {
        oxygine::TouchEvent event(oxygine::TouchEvent::CLICK, false);
        event.target = m_pSpriteBuildingMode;
        event.currentTarget = m_pSpriteBuildingMode;
        m_pSpriteBuildingMode->dispatchEvent(&event);
    }
    else if (cur  == Settings::getKey_EditorPlaceUnit())
    {
        oxygine::TouchEvent event(oxygine::TouchEvent::CLICK, false);
        event.target = m_pSpriteUnitMode;
        event.currentTarget = m_pSpriteUnitMode;
        m_pSpriteUnitMode->dispatchEvent(&event);
    }
    else if (cur  == Settings::getKey_EditorNextTeam())
    {
        spGameMap pMap = GameMap::getInstance();
        qint32 player = 0;
        if (m_currentPlayer.get() != nullptr)
        {
            player = m_currentPlayer->getPlayerID() + 1;
            if (player >= pMap->getPlayerCount())
            {
                player = -1;
            }
        }
        else
        {
            player = 0;
        }
        changeSelectedPlayer(player);
    }
    else if (cur  == Settings::getKey_EditorPreviousTeam())
    {
        spGameMap pMap = GameMap::getInstance();
        qint32 player = 0;
        if (m_currentPlayer.get() != nullptr)
        {
            player = m_currentPlayer->getPlayerID() - 1;
        }
        else
        {
            player = pMap->getPlayerCount() - 1;
        }
        changeSelectedPlayer(player);
    }
    else if (cur == Settings::getKey_EditorSelectionLeft())
    {
        qint32 item = m_selectedItem - 1;
        changeSelection(item);
    }
    else if (cur == Settings::getKey_EditorSelectionRight())
    {
        qint32 item = m_selectedItem + 1;
        changeSelection(item);
    }
}

void EditorSelection::changeSelection(qint32 item)
{
    if (item < 0)
    {
        item = 0;
    }
    switch (m_Mode)
    {
        case EditorMode::Unit:
        {
            if (item >= m_Units.size())
            {
                item = m_Units.size() - 1;
            }
            selectUnit(item);
            break;
        }
        case EditorMode::Terrain:
        {
            if (item >= m_Terrains.size())
            {
                item = m_Terrains.size() - 1;
            }
            selectTerrain(item);
            break;
        }
        case EditorMode::Building:
        {
            if (item >= m_Buildings.size())
            {
                item = m_Buildings.size() - 1;
            }
            selectBuilding(item);
            break;
        }
        case EditorMode::All:
        {
            break;
        }
    }
}

EditorSelection::PlacementSize EditorSelection::getSizeMode() const
{
    return m_SizeMode;
}

spBuilding EditorSelection::getCurrentSpBuilding()
{
    return  m_Buildings.at(m_selectedItem);
}

spUnit EditorSelection::getCurrentSpUnit()
{
    return  m_Units.at(m_selectedItem);
}
