#include "playerinfo.h"

#include "game/gamemap.h"

#include "resource_management/gamemanager.h"

#include "resource_management/cospritemanager.h"

#include "resource_management/fontmanager.h"

#include "game/player.h"

#include "game/co.h"

PlayerInfo::PlayerInfo()
    : QObject()
{
    this->setScale(1.0f);
}

void PlayerInfo::updateData()
{
    // clean up
    this->removeChildren();
    // recreate the ui
    GameMap* pMap = GameMap::getInstance();
    qint32 playerIdx = 0;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (pMap->getCurrentPlayer() == pMap->getPlayer(i))
        {
            playerIdx = i;
            break;
        }
    }
    GameManager* pGameManager = GameManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();

    qint32 yPos = 0;
    qint32 currentPlayer = playerIdx;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        currentPlayer = playerIdx + i;
        if (currentPlayer >= pMap->getPlayerCount())
        {
            currentPlayer -= pMap->getPlayerCount();
        }

        spPlayer pPlayer = pMap->getspPlayer(currentPlayer);
        oxygine::spSprite pSprite = new oxygine::Sprite();
        oxygine::ResAnim* pAnim = nullptr;
        if (pPlayer->getCO(1) == nullptr)
        {
            pAnim = pGameManager->getResAnim("co");
        }
        else
        {
            pAnim = pGameManager->getResAnim("2co");
        }
        pSprite->setResAnim(pAnim);
        QColor color = pPlayer->getColor();
        pSprite->setColor(oxygine::Color(color.red(), color.green(),color.blue()));
        pSprite->setY(yPos);
        this->addChild(pSprite);
        qint32 itemHeigth = pAnim->getHeight() + 5;

        CO* pCO = pPlayer->getCO(0);
        if (pCO == nullptr)
        {
            pAnim = pCOSpriteManager->getResAnim(QString(pCO->getCoID() + "+info").toLower().toStdString().c_str());
        }
        else
        {
            pAnim = pCOSpriteManager->getResAnim("no_co+info"));
        }
        pSprite = new oxygine::Sprite();
        pSprite->setResAnim(pAnim);
        pSprite->setY(yPos);
        pSprite->setScale(2.0f);
        this->addChild(pSprite);
        drawPowerMeter(pCO, pSprite->getY());


        if (pPlayer->getCO(1) != nullptr)
        {
            pCO = pPlayer->getCO(1);
            pAnim = pCOSpriteManager->getResAnim(QString(pCO->getCoID() + "+info").toLower().toStdString().c_str());
            pSprite = new oxygine::Sprite();
            pSprite->setResAnim(pAnim);
            pSprite->setY(yPos + 62);
            drawPowerMeter(pCO, pSprite->getY());
            pSprite->setScale(2.0f);
            this->addChild(pSprite);
        }

        oxygine::TextStyle style = FontManager::getAWStandard();
        oxygine::spTextField Text = new oxygine::TextField();

        Text->setStyle(style);
        QString number = QString::number(pPlayer->getFonds());
        Text->setText(number.toStdString().c_str());
        Text->setY(yPos + 20);
        Text->setX(0);
        Text->setScale(1.0f);
        this->addChild(Text);

        yPos += itemHeigth;

    }
}

void PlayerInfo::drawPowerMeter(CO* pCO, qint32 yPos)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    qint32 power = pCO->getPowerStars();
    qint32 superpower = pCO->getSuperpowerStars();
    bool usePower = pCO->canUsePower();
    bool useSuperpower = pCO->canUseSuperpower();
    float powerFilled = pCO->getPowerFilled();
    switch (pCO->getPowerMode())
    {
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
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else if (powerFilled >= static_cast<float>(i2 + 1) - 0.5f)
                    {
                        pAnim = pGameManager->getResAnim("superpowerstarhalf");
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else
                    {
                        pAnim = pGameManager->getResAnim("superpowerstar");
                    }
                    pSprite->setResAnim(pAnim);
                    pSprite->setY(yPos + 7);
                    pSprite->setX(68 + power * 16 + 20 * (i2 - power));
                    pSprite->setScale(2.0f);
                    if (useSuperpower)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenY(pSprite->getY() - 7), 500, -1, true, i2 * 50);
                        pSprite->addTween(tween);
                    }
                    this->addChild(pSprite);
                }
                else
                {

                    if (powerFilled >= static_cast<float>(i2 + 1))
                    {
                        pAnim = pGameManager->getResAnim("powerstarfull");
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else if (powerFilled >= static_cast<float>(i2 + 1) - 0.5f)
                    {
                        pAnim = pGameManager->getResAnim("powerstarhalf");
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else
                    {
                        pAnim = pGameManager->getResAnim("powerstar");
                    }
                    pSprite->setResAnim(pAnim);
                    pSprite->setY(yPos + 8);
                    pSprite->setX(68 + i2 * 16);
                    pSprite->setScale(2.0f);
                    if (usePower)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenY(pSprite->getY() - 7), 500, -1, true, i2 * 50);
                        pSprite->addTween(tween);
                    }
                    this->addChild(pSprite);
                }
            }
            break;
        }
        case GameEnums::PowerMode_Power:
        {
            oxygine::TextStyle style = FontManager::getMainFont();
            oxygine::spTextField Text = new oxygine::TextField();
            Text->setStyle(style);
            Text->setText(tr("Power").toStdString().c_str());
            Text->setY(yPos);
            Text->setX(68 - 4);
            Text->setScale(1.0f);
            oxygine::Sprite::TweenColor tweenColor(oxygine::Color(0, 0, 255, 255));
            oxygine::spTween tween = oxygine::createTween(tweenColor, 500, -1, true);
            Text->addTween(tween);
            this->addChild(Text);
            break;
        }
        case GameEnums::PowerMode_Superpower:
        {
            oxygine::TextStyle style = FontManager::getMainFont();
            oxygine::spTextField Text = new oxygine::TextField();
            Text->setStyle(style);
            Text->setText(tr("Superpower").toStdString().c_str());
            Text->setY(yPos - 4);
            Text->setX(68);
            Text->setScale(1.0f);
            oxygine::Sprite::TweenColor tweenColor(oxygine::Color(0, 0, 255, 255));
            oxygine::spTween tween = oxygine::createTween(tweenColor, 500, -1, true);
            Text->addTween(tween);
            this->addChild(Text);
            break;
        }
    }
}

