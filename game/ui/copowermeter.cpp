#include "game/ui/copowermeter.h"

#include "game/gamemap.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

CoPowermeter::CoPowermeter(CO* pCO)
    : QObject(),
      _pCO(pCO)
{
    setObjectName("CoPowermeter");
}

void CoPowermeter::drawPowerMeter()
{
    removeChildren();
    if ((GameMap::getInstance() == nullptr ||
        !GameMap::getInstance()->getGameRules()->getNoPower()) &&
        _pCO != nullptr)
    {
        GameManager* pGameManager = GameManager::getInstance();
        oxygine::ResAnim* pAnim = nullptr;
        qint32 power = _pCO->getPowerStars();
        qint32 superpower = _pCO->getSuperpowerStars();
        bool usePower = _pCO->canUsePower();
        bool useSuperpower = _pCO->canUseSuperpower();
        float powerFilled = _pCO->getPowerFilled();
        switch (_pCO->getPowerMode())
        {
            case GameEnums::PowerMode_Unknown:
            case GameEnums::PowerMode_Off:
            {
                for (qint32 i2 = 0; i2 < power + superpower; i2++)
                {
                    oxygine::spSprite pSprite = new oxygine::Sprite();
                    if (i2 >= power)
                    {
                        if (powerFilled >= static_cast<float>(i2 + 1))
                        {
                            pAnim = pGameManager->getResAnim("superpowerstarfull");
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                            pSprite->addTween(tween);
                        }
                        else if (powerFilled >= static_cast<float>(i2 + 1) - 0.5f)
                        {
                            pAnim = pGameManager->getResAnim("superpowerstarhalf");
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
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
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                            pSprite->addTween(tween);
                        }
                        else if (powerFilled >= static_cast<float>(i2 + 1) - 0.5f)
                        {
                            pAnim = pGameManager->getResAnim("powerstarhalf");
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
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
                oxygine::spTextField Text = new oxygine::TextField();
                Text->setStyle(style);
                Text->setHtmlText(tr("Power"));
                Text->setY(0);
                if (m_flippedX)
                {
                    Text->setX(-10 - Text->getTextRect().getWidth());
                }
                else
                {
                    Text->setX(0);
                }
                oxygine::spTweenQueue queue = new oxygine::TweenQueue();
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
                Text->addTween(queue);
                this->addChild(Text);
                break;
            }
            case GameEnums::PowerMode_Superpower:
            {
                oxygine::TextStyle style = FontManager::getMainFont24();
                style.color = FontManager::getFontColor();
                oxygine::spTextField Text = new oxygine::TextField();
                Text->setStyle(style);
                Text->setHtmlText(tr("Superpower"));
                Text->setY(0);
                if (m_flippedX)
                {
                    Text->setX(-10 - Text->getTextRect().getWidth());
                }
                else
                {
                    Text->setX(0);
                }
                oxygine::spTweenQueue queue = new oxygine::TweenQueue();
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
                Text->addTween(queue);
                this->addChild(Text);
                break;
            }
            case GameEnums::PowerMode_Tagpower:
            {
                oxygine::TextStyle style = FontManager::getMainFont24();
                style.color = FontManager::getFontColor();
                oxygine::spTextField Text = new oxygine::TextField();
                Text->setStyle(style);
                Text->setHtmlText(tr("Tagpower"));
                Text->setY(- 4);
                if (m_flippedX)
                {
                    Text->setX(-10 - Text->getTextRect().getWidth());
                }
                else
                {
                    Text->setX(0);
                }
                oxygine::spTweenQueue queue = new oxygine::TweenQueue();
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
                Text->addTween(queue);
                this->addChild(Text);
                break;
            }
        }
    }
}

CO *CoPowermeter::getCO() const
{
    return _pCO;
}

void CoPowermeter::setCO(CO *pCO)
{
    _pCO = pCO;
}

bool CoPowermeter::getFlippedX() const
{
    return m_flippedX;
}

void CoPowermeter::setFlippedX(bool value)
{
    m_flippedX = value;
}
