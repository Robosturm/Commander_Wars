#include "game/ui/playerinfo.h"

#include "game/gamemap.h"

#include "resource_management/gamemanager.h"

#include "resource_management/cospritemanager.h"

#include "resource_management/fontmanager.h"

#include "game/player.h"

#include "game/co.h"

#include "game/ui/copowermeter.h"

PlayerInfo::PlayerInfo(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("PlayerInfo");
#endif
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setScale(1.0f);
}

void PlayerInfo::updateData()
{    
    Mainapp::getInstance()->pauseRendering();
    // clean up
    removeChildren();
    // recreate the ui
    
    qint32 playerIdx = 0;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if (m_pMap->getCurrentPlayer() == m_pMap->getPlayer(i))
        {
            playerIdx = i;
            break;
        }
    }
    GameManager* pGameManager = GameManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();

    Player* pViewPlayer = m_pMap->getCurrentViewPlayer();
    qint32 yPos = 0;
    qint32 currentPlayer = playerIdx;
    qint32 count = m_pMap->getPlayerCount();
    qint32 maxCount = count;
    if (Settings::getShowCoCount() > 0 &&
        Settings::getShowCoCount() < m_pMap->getPlayerCount())
    {
        maxCount = Settings::getShowCoCount();
    }
    qint32 playerShown = 0;
    for (qint32 i = 0; i < count; i++)
    {
        currentPlayer = playerIdx + i;
        if (currentPlayer >= m_pMap->getPlayerCount())
        {
            currentPlayer -= m_pMap->getPlayerCount();
        }

        spPlayer pPlayer = m_pMap->getspPlayer(currentPlayer);
        // draw player if he is alive
        if (!pPlayer->getIsDefeated())
        {
            playerShown++;
            oxygine::spSprite pSprite = oxygine::spSprite::create();
            oxygine::ResAnim* pAnim = nullptr;
            if (pPlayer->getCO(1) == nullptr)
            {
                pAnim = pGameManager->getResAnim("co");
            }
            else
            {
                pAnim = pGameManager->getResAnim("2co");
            }
            if (pAnim != nullptr)
            {
                pSprite->setResAnim(pAnim);
                pSprite->setColorTable(pPlayer->getColorTableAnim(), true);
                pSprite->setY(yPos);
                pSprite->setFlippedX(m_flippedX);
                if (m_flippedX)
                {
                    pSprite->setX(-pSprite->getScaledWidth());
                }
                addChild(pSprite);
                qint32 itemHeigth = static_cast<qint32>(pAnim->getHeight()) + 5;

                CO* pCO = pPlayer->getCO(0);
                if (pCO != nullptr)
                {
                    pAnim = pCO->getResAnim(QString(pCO->getCoID() + "+info").toLower());
                }
                else
                {
                    pAnim = pCOSpriteManager->getResAnim("no_co+info");
                }
                pSprite = oxygine::spSprite::create();
                pSprite->setResAnim(pAnim);
                pSprite->setY(yPos);
                if (pAnim != nullptr)
                {
                    pSprite->setScale(2.0f * pAnim->getWidth() / 32.0f);
                }
                else
                {
                    pSprite->setScale(2.0f);
                }
                pSprite->setFlippedX(m_flippedX);
                if (m_flippedX)
                {
                    pSprite->setX(-pSprite->getScaledWidth());
                }
                addChild(pSprite);
                if (pCO != nullptr)
                {
                    spCoPowermeter pCoPowermeter = spCoPowermeter::create(m_pMap, pCO);
                    pCoPowermeter->setY(pSprite->getY());
                    pCoPowermeter->setFlippedX(m_flippedX);
                    if (m_flippedX)
                    {
                        pCoPowermeter->setX(-68);
                    }
                    else
                    {
                        pCoPowermeter->setX(68);
                    }
                    pCoPowermeter->drawPowerMeter();
                    addChild(pCoPowermeter);
                }

                if (pPlayer->getCO(1) != nullptr)
                {
                    pCO = pPlayer->getCO(1);
                    pAnim = pCO->getResAnim(QString(pCO->getCoID() + "+info").toLower());
                    pSprite = oxygine::spSprite::create();
                    pSprite->setResAnim(pAnim);
                    pSprite->setY(yPos + 62);
                    spCoPowermeter pCoPowermeter = spCoPowermeter::create(m_pMap, pCO);
                    pCoPowermeter->setY(pSprite->getY());
                    pCoPowermeter->setFlippedX(m_flippedX);
                    pCoPowermeter->drawPowerMeter();
                    addChild(pCoPowermeter);
                    if (pAnim != nullptr)
                    {
                        pSprite->setScale(2.0f * pAnim->getWidth() / 32.0f);
                    }
                    else
                    {
                        pSprite->setScale(2.0f);
                    }
                    pSprite->setFlippedX(m_flippedX);
                    if (m_flippedX)
                    {
                        pCoPowermeter->setX(-68);
                        pSprite->setX(-pSprite->getScaledWidth());
                    }
                    else
                    {
                        pCoPowermeter->setX(68);
                    }
                    addChild(pSprite);
                }
                oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());

                oxygine::spTextField Text = oxygine::spTextField::create();

                Text->setStyle(style);
                QString number = QString::number(pPlayer->getFunds());
                if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                    m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off &&
                    m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_OfMist)
                {
                    number = "?";
                }
                Text->setHtmlText(number);
                Text->setY(yPos + 30);
                if (m_flippedX)
                {
                    Text->setX(-10 - Text->getTextRect().getWidth());
                }
                else
                {
                    Text->setX(0);
                }
                addChild(Text);

                yPos += itemHeigth;
            }
        }
        if (playerShown >= maxCount)
        {
            break;
        }
    }
    setHeight(yPos);
    Mainapp::getInstance()->continueRendering();
}

bool PlayerInfo::getFlippedX() const
{
    return m_flippedX;
}

GameMap *PlayerInfo::getMap() const
{
    return m_pMap;
}

void PlayerInfo::setFlippedX(bool value)
{
    m_flippedX = value;
}
