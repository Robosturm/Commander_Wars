#include "game/ui/copowermeter.h"

#include "game/gamemap.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

CoPowermeter::CoPowermeter(CO* pCO)
    : QObject(),
      m_pCO(pCO)
{
    setObjectName("CoPowermeter");
}

void CoPowermeter::drawPowerMeter()
{
    removeChildren();
    spGameMap pMap = GameMap::getInstance();
    if ((pMap.get() == nullptr ||
        !pMap->getGameRules()->getNoPower()) &&
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
                        this->addChild(pSprite);
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
                        this->addChild(pSprite);
                    }
                }
                break;
            }
            case GameEnums::PowerMode_Power:
            {
                oxygine::TextStyle style = FontManager::getMainFont24();
                style.color = FontManager::getFontColor();
                oxygine::spTextField pText = oxygine::spTextField::create();
                pText->setStyle(style);
                pText->setHtmlText(tr("Power"));
                pText->setY(0);
                if (m_flippedX)
                {
                    pText->setX(-10 - pText->getTextRect().getWidth());
                }
                else
                {
                    pText->setX(0);
                }
                oxygine::spTweenQueue queue = oxygine::spTweenQueue::create();
                oxygine::Sprite::TweenColor tweenColor1(QColor(255, 255, 255, 255));
                oxygine::spTween tween1 = oxygine::createTween(tweenColor1, oxygine::timeMS(200), 1, false);
                oxygine::Sprite::TweenColor tweenColor2(QColor(255, 0, 0, 255));
                oxygine::spTween tween2 = oxygine::createTween(tweenColor2, oxygine::timeMS(200), 1, false);
                oxygine::Sprite::TweenColor tweenColor3(QColor(0, 255, 0, 255));
                oxygine::spTween tween3 = oxygine::createTween(tweenColor3, oxygine::timeMS(200), 1, false);
                queue->add(tween1);
                queue->add(tween2);
                queue->add(tween3);
                queue->setLoops(-1);
                pText->addTween(queue);
                this->addChild(pText);
                break;
            }
            case GameEnums::PowerMode_Superpower:
            {
                oxygine::TextStyle style = FontManager::getMainFont24();
                style.color = FontManager::getFontColor();
                oxygine::spTextField pText = oxygine::spTextField::create();
                pText->setStyle(style);
                pText->setHtmlText(tr("Superpower"));
                pText->setY(0);
                if (m_flippedX)
                {
                    pText->setX(-10 - pText->getTextRect().getWidth());
                }
                else
                {
                    pText->setX(0);
                }
                oxygine::spTweenQueue queue = oxygine::spTweenQueue::create();
                oxygine::Sprite::TweenColor tweenColor1(QColor(255, 255, 255, 255));
                oxygine::spTween tween1 = oxygine::createTween(tweenColor1, oxygine::timeMS(200), 1, false);
                oxygine::Sprite::TweenColor tweenColor2(QColor(255, 0, 0, 255));
                oxygine::spTween tween2 = oxygine::createTween(tweenColor2, oxygine::timeMS(200), 1, false);
                oxygine::Sprite::TweenColor tweenColor3(QColor(0, 255, 0, 255));
                oxygine::spTween tween3 = oxygine::createTween(tweenColor3, oxygine::timeMS(200), 1, false);
                queue->add(tween1);
                queue->add(tween2);
                queue->add(tween3);
                queue->setLoops(-1);
                pText->addTween(queue);
                this->addChild(pText);
                break;
            }
            case GameEnums::PowerMode_Tagpower:
            {
                oxygine::TextStyle style = FontManager::getMainFont24();
                style.color = FontManager::getFontColor();
                oxygine::spTextField pText = oxygine::spTextField::create();
                pText->setStyle(style);
                pText->setHtmlText(tr("Tagpower"));
                pText->setY(- 4);
                if (m_flippedX)
                {
                    pText->setX(-10 - pText->getTextRect().getWidth());
                }
                else
                {
                    pText->setX(0);
                }
                oxygine::spTweenQueue queue = oxygine::spTweenQueue::create();
                oxygine::Sprite::TweenColor tweenColor1(QColor(255, 255, 255, 255));
                oxygine::spTween tween1 = oxygine::createTween(tweenColor1, oxygine::timeMS(100), 1, false);
                oxygine::Sprite::TweenColor tweenColor2(QColor(255, 0, 0, 255));
                oxygine::spTween tween2 = oxygine::createTween(tweenColor2, oxygine::timeMS(100), 1, false);
                oxygine::Sprite::TweenColor tweenColor3(QColor(0, 255, 0, 255));
                oxygine::spTween tween3 = oxygine::createTween(tweenColor3, oxygine::timeMS(100), 1, false);
                queue->add(tween1);
                queue->add(tween2);
                queue->add(tween3);
                queue->setLoops(-1);
                pText->addTween(queue);
                this->addChild(pText);
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

void CoPowermeter::setFlippedX(bool value)
{
    m_flippedX = value;
}
