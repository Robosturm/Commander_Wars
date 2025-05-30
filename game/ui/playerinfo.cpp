#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"
#include "game/ui/copowermeter.h"
#include "game/ui/playerinfo.h"

PlayerInfo::PlayerInfo(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("PlayerInfo");
#endif
    CONSOLE_PRINT("Creating PlayerInfo", GameConsole::eDEBUG);
    Interpreter::setCppOwnerShip(this);
}

void PlayerInfo::updateData()
{    
    Mainapp::getInstance()->pauseRendering();
    if (m_pMap != nullptr)
    {
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
        if (Settings::getInstance()->getShowCoCount() > 0 &&
            Settings::getInstance()->getShowCoCount() < m_pMap->getPlayerCount())
        {
            maxCount = Settings::getInstance()->getShowCoCount();
        }
        qint32 playerShown = 0;
        bool hasShownTurnStartInfo = false;
        qint32 height = Settings::getInstance()->getHeight();
        for (qint32 i = 0; i < count && yPos < height; i++)
        {
            currentPlayer = playerIdx + i;
            if (currentPlayer >= m_pMap->getPlayerCount())
            {
                currentPlayer -= m_pMap->getPlayerCount();
            }

            if (currentPlayer == 0)
            {
                showTurnStartInfo(yPos);
                hasShownTurnStartInfo = true;
            }

            spPlayer pPlayer = m_pMap->getSpPlayer(currentPlayer);
            // draw player if he is alive
            if (!pPlayer->getIsDefeated())
            {
                playerShown++;
                oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
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

                    spCO pCO = pPlayer->getSpCO(0);
                    if (pCO.get() != nullptr)
                    {
                        QString resAnim = pCO->getCoID() + "+info";
                        pAnim = pCO->getResAnim(resAnim.toLower());
                    }
                    else
                    {
                        pAnim = pCOSpriteManager->getResAnim("no_co+info");
                    }
                    pSprite = MemoryManagement::create<oxygine::Sprite>();
                    pSprite->setResAnim(pAnim);
                    if (pAnim != nullptr)
                    {
                        pSprite->setWidth(pAnim->getWidth());
                    }
                    pSprite->setY(yPos);
                    if (pAnim != nullptr)
                    {
                        pSprite->setScale(2.0f * static_cast<float>(pAnim->getWidth()) / 32.0f);
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

                    // online info
                    qint32 coWidth = pSprite->getScaledWidth();
                    pSprite = MemoryManagement::create<oxygine::Sprite>();
                    if (pPlayer->getIsOnline())
                    {
                        pAnim = pGameManager->getResAnim("online");
                    }
                    else
                    {
                        pAnim = pGameManager->getResAnim("offline");
                    }
                    pSprite->setResAnim(pAnim);
                    pSprite->setY(yPos);
                    pSprite->setX(coWidth + 5);
                    pSprite->setFlippedX(m_flippedX);
                    if (m_flippedX)
                    {
                        pSprite->setX(-pSprite->getScaledWidth() - 5 - coWidth);
                    }
                    addChild(pSprite);

                    if (pCO != nullptr)
                    {
                        spCoPowermeter pCoPowermeter = MemoryManagement::create<CoPowermeter>(m_pMap, pCO.get());
                        pCoPowermeter->setY(pSprite->getY() + 3);
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

                    pCO = pPlayer->getSpCO(1);
                    if (pCO.get() != nullptr)
                    {
                        QString resAnim = pCO->getCoID() + "+info";
                        pAnim = pCO->getResAnim(resAnim.toLower());
                        pSprite = MemoryManagement::create<oxygine::Sprite>();
                        pSprite->setResAnim(pAnim);
                        pSprite->setY(yPos + 62);
                        spCoPowermeter pCoPowermeter = MemoryManagement::create<CoPowermeter>(m_pMap, pCO.get());
                        pCoPowermeter->setY(pSprite->getY() + 3);
                        pCoPowermeter->setFlippedX(m_flippedX);
                        pCoPowermeter->drawPowerMeter();
                        addChild(pCoPowermeter);
                        if (pAnim != nullptr)
                        {
                            pSprite->setScale(2.0f * static_cast<float>(pAnim->getWidth()) / 32.0f);
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
                    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getFont("fundsInfo24"));
                    oxygine::spTextField Text = MemoryManagement::create<oxygine::TextField>();
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
                        Text->setX(-Text->getTextRect().width());
                    }
                    else
                    {
                        Text->setX(3);
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

        if (!hasShownTurnStartInfo)
        {
            showTurnStartInfo(yPos);
        }
        setHeight(yPos);
    }
    Mainapp::getInstance()->continueRendering();
}

void PlayerInfo::showTurnStartInfo(qint32 & yPos)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("turnStartInfo");
    Player* pPlayer = m_pMap->getPlayer(0);
    if (pAnim != nullptr && pPlayer != nullptr)
    {
        qint32 itemHeigth = static_cast<qint32>(pAnim->getHeight()) + 5;
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pAnim);
        pSprite->setColorTable(pPlayer->getColorTableAnim(), true);
        pSprite->setY(yPos);
        pSprite->setFlippedX(m_flippedX);
        if (m_flippedX)
        {
            pSprite->setX(-pSprite->getScaledWidth());
        }
        addChild(pSprite);
        if (m_pMap->getGameRules()->getWeatherPrediction())
        {
            qint32 x = 2;
            pSprite = MemoryManagement::create<oxygine::Sprite>();
            Weather* pWeather = m_pMap->getGameRules()->getWeatherAtDay(1, 0);
            if (pWeather != nullptr)
            {
                pAnim = pGameManager->getResAnim((pWeather->getWeatherSymbol()));
                if (pAnim != nullptr)
                {
                    pSprite->setResAnim(pAnim);
                    pSprite->setY(yPos + 2);
                    pSprite->setScale(16.0f / static_cast<float>(pAnim->getWidth()));
                    if (m_flippedX)
                    {
                        x = -4 - pSprite->getScaledWidth();
                        pSprite->setX(-pSprite->getScaledWidth() - 2);
                    }
                    else
                    {
                        x = 4 + pSprite->getScaledWidth();
                        pSprite->setX(2);
                    }
                    addChild(pSprite);
                }
            }

            oxygine::TextStyle style = oxygine::TextStyle(FontManager::getFont("nextDayInfo16"));
            oxygine::spTextField pText = MemoryManagement::create<oxygine::TextField>();
            pText->setStyle(style);
            QString number = QString::number(m_pMap->getCurrentDay() + 1);
            pText->setHtmlText(tr("Next day ") + number);
            pText->setY(yPos - 4);
            if (m_flippedX)
            {
                pText->setX(x - pText->getTextRect().width());
            }
            else
            {
                pText->setX(x);
            }
            addChild(pText);
        }
        yPos += itemHeigth;
    }
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
