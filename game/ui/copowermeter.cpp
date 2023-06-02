#include "game/ui/copowermeter.h"

#include "game/gamemap.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

CoPowermeter::CoPowermeter(GameMap* pMap, CO* pCO)
    : m_pCO(pCO),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("CoPowermeter");
#endif
    Interpreter::setCppOwnerShip(this);
}

void CoPowermeter::drawPowerMeter()
{
    removeChildren();
    
    if ((m_pMap == nullptr ||
        !m_pMap->getGameRules()->getNoPower()) &&
        m_pCO != nullptr)
    {
        GameManager* pGameManager = GameManager::getInstance();
        oxygine::ResAnim* pAnim = nullptr;
        qint32 power = m_pCO->getPowerStars();
        qint32 superpower = m_pCO->getSuperpowerStars();
        bool usePower = m_pCO->canUsePower();
        bool useSuperpower = m_pCO->canUseSuperpower();
        float powerFilled = m_pCO->getPowerFilled();
        switch (m_pCO->getPowerMode())
        {
            case GameEnums::PowerMode_Unknown:
            case GameEnums::PowerMode_Off:
            {
                for (qint32 i2 = 0; i2 < power + superpower; i2++)
                {
                    oxygine::spSprite pSprite = oxygine::spSprite::create();
                    if (i2 >= power)
                    {
                        if (powerFilled >= static_cast<float>(i2 + 1))
                        {
                            pAnim = pGameManager->getResAnim("superpowerstarfull");
                            qint32 totalFrames = 0;
                            if (pAnim != nullptr)
                            {
                                totalFrames = pAnim->getTotalFrames();
                            }
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(totalFrames * GameMap::frameTime), -1);
                            pSprite->addTween(tween);
                        }
                        else if (powerFilled >= static_cast<float>(i2 + 1) - 0.5f)
                        {
                            pAnim = pGameManager->getResAnim("superpowerstarhalf");
                            qint32 totalFrames = 0;
                            if (pAnim != nullptr)
                            {
                                totalFrames = pAnim->getTotalFrames();
                            }
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(totalFrames * GameMap::frameTime), -1);
                            pSprite->addTween(tween);
                        }
                        else
                        {
                            pAnim = pGameManager->getResAnim("superpowerstar");
                        }
                        pSprite->setResAnim(pAnim);
                        pSprite->setY(7);
                        qint32 x = power * 16 + 20 * (i2 - power);
                        if (m_flippedX)
                        {
                            pSprite->setX(-x - 16);
                        }
                        else
                        {
                            pSprite->setX(x);
                        }
                        pSprite->setScale(2.0f);
                        if (useSuperpower)
                        {
                            oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenY(pSprite->getY() - 7), oxygine::timeMS(500), -1, true, oxygine::timeMS(i2 * 50));
                            pSprite->addTween(tween);
                        }
                        addChild(pSprite);
                    }
                    else
                    {

                        if (powerFilled >= static_cast<float>(i2 + 1))
                        {
                            pAnim = pGameManager->getResAnim("powerstarfull");
                            qint32 totalFrames = 0;
                            if (pAnim != nullptr)
                            {
                                totalFrames = pAnim->getTotalFrames();
                            }
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(totalFrames * GameMap::frameTime), -1);
                            pSprite->addTween(tween);
                        }
                        else if (powerFilled >= static_cast<float>(i2 + 1) - 0.5f)
                        {
                            pAnim = pGameManager->getResAnim("powerstarhalf");
                            qint32 totalFrames = 0;
                            if (pAnim != nullptr)
                            {
                                totalFrames = pAnim->getTotalFrames();
                            }
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(totalFrames * GameMap::frameTime), -1);
                            pSprite->addTween(tween);
                        }
                        else
                        {
                            pAnim = pGameManager->getResAnim("powerstar");
                        }
                        pSprite->setResAnim(pAnim);
                        pSprite->setY(8);
                        qint32 x = i2 * 16;
                        if (m_flippedX)
                        {
                            pSprite->setX(-x - 16);
                        }
                        else
                        {
                            pSprite->setX(x);
                        }
                        pSprite->setScale(2.0f);
                        if (usePower)
                        {
                            oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenY(pSprite->getY() - 7), oxygine::timeMS(500), -1, true, oxygine::timeMS(i2 * 50));
                            pSprite->addTween(tween);
                        }
                        addChild(pSprite);
                    }
                }
                break;
            }
            case GameEnums::PowerMode_Power:
            {
                oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
                oxygine::spTextField pText = oxygine::spTextField::create();
                pText->setStyle(style);
                pText->setHtmlText(tr("<wiggly speed=\"60\" advance=\"3\" maxY=\"5\">Power</wiggly>"));
                pText->setY(0);
                if (m_flippedX)
                {
                    pText->setX(-10 - pText->getTextRect().width());
                }
                else
                {
                    pText->setX(0);
                }
                addChild(pText);
                break;
            }
            case GameEnums::PowerMode_Superpower:
            {
                oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
                oxygine::spTextField pText = oxygine::spTextField::create();
                pText->setStyle(style);
                pText->setHtmlText(tr("<wiggly speed=\"60\" advance=\"2\" maxY=\"5\">Superpower</wiggly>"));
                pText->setY(0);
                if (m_flippedX)
                {
                    pText->setX(-10 - pText->getTextRect().width());
                }
                else
                {
                    pText->setX(0);
                }
                addChild(pText);
                break;
            }
            case GameEnums::PowerMode_Tagpower:
            {
                oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
                oxygine::spTextField pText = oxygine::spTextField::create();
                pText->setStyle(style);
                pText->setHtmlText(tr("<wiggly speed=\"60\" advance=\"2\" maxY=\"5\">Tagpower</wiggly>"));
                pText->setY(- 4);
                if (m_flippedX)
                {
                    pText->setX(-10 - pText->getTextRect().width());
                }
                else
                {
                    pText->setX(0);
                }
                addChild(pText);
                break;
            }
        }
    }
}

CO *CoPowermeter::getCO() const
{
    return m_pCO;
}

void CoPowermeter::setCO(CO *pCO)
{
    m_pCO = pCO;
}

bool CoPowermeter::getFlippedX() const
{
    return m_flippedX;
}

GameMap *CoPowermeter::getMap() const
{
    return m_pMap;
}

void CoPowermeter::setMap(GameMap *newPMap)
{
    m_pMap = newPMap;
}

void CoPowermeter::setFlippedX(bool value)
{
    m_flippedX = value;
}
