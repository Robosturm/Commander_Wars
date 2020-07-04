#include "minimap.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/unit.h"

#include "game/building.h"

#include "game/co.h"

#include "coreengine/tweentogglevisibility.h"

#include "resource_management/gamemanager.h"
#include "menue/gamemenue.h"

Minimap::Minimap()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            float x = pTouchEvent->localPosition.x / ImageSize;
            float y = pTouchEvent->localPosition.y / ImageSize;
            emit clicked(static_cast<qint32>(x), static_cast<qint32>(y));
        }
    });
}

Minimap::~Minimap()
{
}

void Minimap::updateMinimap(spGameMap pMap, bool useVision)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // clear minimap
    removeChildren();
    // load new minimap
    if (pMap.get() != nullptr)
    {
        qint32 width = static_cast<qint32>(pMap->getMapWidth());
        qint32 heigth = static_cast<qint32>(pMap->getMapHeight());
        setSize(ImageSize * width, ImageSize * heigth);
        Player* pPlayer = pMap->getCurrentPlayer();
        spGameMenue pGamemenu = GameMenue::getInstance();
        if (pGamemenu.get() != nullptr)
        {
            pPlayer = pGamemenu->getCurrentViewPlayer();
        }

        GameManager* pGameManager = GameManager::getInstance();
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                if (pPlayer->getFieldVisibleType(x, y) == GameEnums::VisionType_Shrouded &&
                    pMap->getGameRules()->getFogMode() == GameEnums::Fog_OfShroud)
                {
                    oxygine::spColorRectSprite pSprite = new oxygine::ColorRectSprite();
                    pSprite->setColor(Qt::black);
                    pSprite->setPosition(x * ImageSize, y * ImageSize);
                    pSprite->setSize(ImageSize, ImageSize);
                    addChild(pSprite);
                }
                else
                {
                    Terrain* pTerrain = pMap->getTerrain(x, y);
                    Building* pBuilding = pTerrain->getBuilding();
                    if (pBuilding != nullptr)
                    {
                        oxygine::ResAnim* pAnim = pGameManager->getResAnim(pBuilding->getMinimapIcon());
                        if (pAnim != nullptr)
                        {
                            oxygine::spSprite pSprite = new oxygine::Sprite();
                            if (pAnim->getTotalFrames() > 1)
                            {
                                oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                                pSprite->addTween(tween);
                            }
                            else
                            {
                                pSprite->setResAnim(pAnim);
                            }
                            pSprite->setPosition(x * ImageSize, y * ImageSize);
                            Player* pPlayer = pBuilding->getOwner();
                            if (pPlayer != nullptr && !pBuilding->getNeutralLoaded())
                            {
                                QColor color = pPlayer->getColor();
                                pSprite->setColor(color.red(), color.green(), color.blue(), 255);
                            }
                            addChild(pSprite);
                        }
                    }
                    else
                    {
                        oxygine::ResAnim* pAnim = pGameManager->getResAnim(pTerrain->getMinimapIcon());
                        if (pAnim != nullptr)
                        {
                            oxygine::spSprite pSprite = new oxygine::Sprite();
                            if (pAnim->getTotalFrames() > 1)
                            {
                                oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                                pSprite->addTween(tween);
                            }
                            else
                            {
                                pSprite->setResAnim(pAnim);
                            }
                            pSprite->setPosition(x * ImageSize, y * ImageSize);
                            addChild(pSprite);
                        }
                    }
                    Unit* pUnit = pTerrain->getUnit();
                    if (pUnit != nullptr)
                    {
                        if (!useVision || !pUnit->isStealthed(pPlayer))
                        {
                            oxygine::ResAnim* pAnim = pGameManager->getResAnim("minimap_unit");
                            if (pAnim != nullptr)
                            {
                                oxygine::spSprite pSprite = new oxygine::Sprite();
                                if (pAnim->getTotalFrames() > 1)
                                {
                                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                                    pSprite->addTween(tween);
                                }
                                else
                                {
                                    pSprite->setResAnim(pAnim);
                                }
                                oxygine::spTween tween2 = oxygine::createTween(TweenToggleVisibility(0, 0.5f), oxygine::timeMS(1000), -1);
                                pSprite->addTween(tween2);
                                pSprite->setPosition(x * ImageSize, y * ImageSize);
                                Player* pPlayer = pUnit->getOwner();
                                QColor color = pPlayer->getColor();
                                pSprite->setColor(color.red(), color.green(), color.blue(), 255);
                                pSprite->setPriority(1);
                                addChild(pSprite);
                            }
                        }
                    }
                }
            }
        }
    }
    pApp->continueThread();
}
