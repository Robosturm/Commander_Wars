#include "objects/dialogs/ingame/dialogattacklog.h"

#include "coreengine/mainapp.h"

#include "game/gamerecording/gamerecorder.h"
#include "game/player.h"
#include "game/gamemap.h"
#include "gameinput/humanplayerinput.h"

#include "objects/base/panel.h"
#include "objects/base/label.h"

#include "resource_management/objectmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

DialogAttackLog::DialogAttackLog(GameMap* pMap, Player* pPlayer)
    : m_pPlayer(pPlayer),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogAttackLog");
#endif
    Interpreter::setCppOwnerShip(this);
    m_Log = m_pMap->getGameRecorder()->getAttackLog(pPlayer->getPlayerID());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() / 2 - pOkButton->getScaledWidth() / 2,
                           Settings::getHeight() - 10 - pOkButton->getScaledHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });

    spPanel pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 150));
    pPanel->setPosition(30, 70);

    pSpriteBox->addChild(pPanel);
    qint32 y = 30;
    spLabel pText = spLabel::create(130);
    pText->setStyle(style);
    pText->setHtmlText(tr("Attacker"));
    pText->setPosition(10 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText(tr("X"));
    pText->setPosition(140 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText(tr("Y"));
    pText->setPosition(240 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(100);
    pText->setStyle(style);
    pText->setHtmlText(tr("Dealt"));
    pText->setPosition(330 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(130);
    pText->setStyle(style);
    pText->setHtmlText(tr("Defender"));
    pText->setPosition(440 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText(tr("X"));
    pText->setPosition(580 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText(tr("Y"));
    pText->setPosition(670 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(100);
    pText->setStyle(style);
    pText->setHtmlText(tr("Dealt"));
    pText->setPosition(760 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    qint32 currentDay = -1;
    y = 10;
    for (qint32 i = m_Log.size() - 1; i >= 0; i--)
    {
        const spAttackReport& log = m_Log[i];
        if (log->day != currentDay)
        {
            currentDay = log->day;
            spLabel pText = spLabel::create(pPanel->getContentWidth() - 70);
            pText->setHtmlText(tr("Day ") + QString::number(currentDay));
            pText->setStyle(style);
            pText->setPosition(10, y);
            pPanel->addItem(pText);
            y += pText->getHeight() + 10;
        }

        Terrain* pTerrain = m_pMap->getTerrain(log->attackerX, log->attackerY);
        spTerrain pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "", m_pMap);
        pActor->loadSprites();
        Building* pBuilding = pTerrain->getBuilding();
        if (pBuilding != nullptr)
        {
            spBuilding pTerrainBuilding = spBuilding::create(pBuilding->getBuildingID(), m_pMap);
            pTerrainBuilding->setOwner(nullptr);
            pTerrainBuilding->scaleAndShowOnSingleTile();
            pActor->addChild(pTerrainBuilding);
        }
        pActor->addChild(spUnit::create(log->attackerID, m_pMap->getPlayer(log->attackerOwnerID), false, m_pMap));
        pActor->setPosition(60, y + 8);
        if (log->attackerKilled)
        {
            oxygine::ResAnim* pAnim = pGameManager->getResAnim("icon_fire");
            if (pAnim != nullptr)
            {
                oxygine::spSprite pSprite = oxygine::spSprite::create();
                pSprite->setResAnim(pAnim);
                pSprite->setScale(GameMap::getImageSize() / pAnim->getWidth() * 0.75f);
                pSprite->setPosition(0, GameMap::getImageSize() * 1.0f / 4.0f);
                pActor->addChild(pSprite);
            }
        }
        pPanel->addItem(pActor);

        pText = spLabel::create(80);
        pText->setStyle(style);
        bool isDefender = (log->defenderOwnerID == pPlayer->getPlayerID());
        if (log->attackerX >= 0 &&
            ((log->defenderSeesAttacker && isDefender) || !isDefender))
        {
            pText->setHtmlText(QString::number(log->attackerX));
        }
        else
        {
            pText->setHtmlText(tr("?"));
        }
        pText->setPosition(130, y);
        pPanel->addItem(pText);

        pText = spLabel::create(80);
        pText->setStyle(style);
        if (log->attackerY >= 0 &&
            ((log->defenderSeesAttacker && isDefender) || !isDefender))
        {
            pText->setHtmlText(QString::number(log->attackerY));
        }
        else
        {
            pText->setHtmlText(tr("?"));
        }
        pText->setPosition(230, y);
        pPanel->addItem(pText);

        pText = spLabel::create(100);
        pText->setStyle(style);
        pText->setHtmlText(QString::number(log->attackerDamage) + tr("Hp"));
        pText->setPosition(320, y);
        pPanel->addItem(pText);

        pTerrain = m_pMap->getTerrain(log->defenderX, log->defenderY);
        pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "", m_pMap);
        pBuilding = pTerrain->getBuilding();
        pActor->loadSprites();
        if (pBuilding != nullptr)
        {
            spBuilding pTerrainBuilding = spBuilding::create(pBuilding->getBuildingID(), m_pMap);
            pTerrainBuilding->setOwner(nullptr);
            pTerrainBuilding->scaleAndShowOnSingleTile();
            pActor->addChild(pTerrainBuilding);
        }
        pActor->addChild(spUnit::create(log->defenderID, m_pMap->getPlayer(log->defenderOwnerID), false, m_pMap));
        if (log->defenderKilled)
        {
            oxygine::ResAnim* pAnim = pGameManager->getResAnim("icon_fire");
            if (pAnim != nullptr)
            {
                oxygine::spSprite pSprite = oxygine::spSprite::create();
                pSprite->setResAnim(pAnim);
                pSprite->setScale(GameMap::getImageSize() / pAnim->getWidth() * 0.75f);
                pSprite->setPosition(0, GameMap::getImageSize() * 1.0f / 4.0f);
                pActor->addChild(pSprite);
            }
        }
        pActor->setPosition(500, y + 8);
        pPanel->addItem(pActor);

        pText = spLabel::create(80);
        pText->setStyle(style);
        if (log->defenderX >= 0)
        {
            pText->setHtmlText(QString::number(log->defenderX));
        }
        else
        {
            pText->setHtmlText(tr("?"));
        }
        pText->setPosition(570, y);
        pPanel->addItem(pText);

        pText = spLabel::create(80);
        pText->setStyle(style);
        if (log->defenderY >= 0)
        {
            pText->setHtmlText(QString::number(log->defenderY));
        }
        else
        {
            pText->setHtmlText(tr("?"));
        }
        pText->setPosition(660, y);
        pPanel->addItem(pText);

        pText = spLabel::create(100);
        pText->setStyle(style);
        pText->setHtmlText(QString::number(log->defenderDamage) + tr("Hp"));
        pText->setPosition(750, y);
        pPanel->addItem(pText);

        oxygine::spButton pButton = ObjectManager::createButton(tr("Show"), 100);
        pButton->setPosition(860, y);

        // to copy less data for the lambda
        qint32 posAtkX = log->attackerX;
        qint32 posAtkY = log->attackerY;
        if (!log->defenderSeesAttacker && isDefender)
        {
            posAtkX = -1;
            posAtkY = -1;
        }
        qint32 playerAtk = log->attackerOwnerID;
        qint32 posDefX = log->defenderX;
        qint32 posDefY = log->defenderY;
        qint32 playerDef = log->defenderOwnerID;
        pButton->addClickListener([this, posAtkX, posAtkY, playerAtk, posDefX, posDefY, playerDef](oxygine::Event*)
        {
            emit sigShowAttack(posAtkX, posAtkY, playerAtk, posDefX, posDefY, playerDef);
        });
        pPanel->addItem(pButton);
        y += pText->getHeight() + 10;
    }
    pPanel->setContentWidth(1000);
    pPanel->setContentHeigth(y + 40);

    connect(this, &DialogAttackLog::sigShowAttack, this, &DialogAttackLog::showAttack, Qt::QueuedConnection);
    connect(this, &DialogAttackLog::sigFinished, this, &DialogAttackLog::remove, Qt::QueuedConnection);
}

void DialogAttackLog::remove()
{
    detach();
}

void DialogAttackLog::showAttack(qint32 posAtkX, qint32 posAtkY, qint32 playerAtk, qint32 posDefX, qint32 posDefY, qint32 playerDef)
{
    
    
    HumanPlayerInput* pInput = dynamic_cast<HumanPlayerInput*>(m_pMap->getCurrentPlayer()->getBaseGameInput());
    if (pInput != nullptr)
    {
        pInput->createMarkedField(QPoint(posAtkX, posAtkY), m_pMap->getPlayer(playerAtk)->getColor());
        pInput->createMarkedField(QPoint(posDefX, posDefY), m_pMap->getPlayer(playerDef)->getColor());
    }
    emit sigFinished();
    oxygine::Actor::detach();
}
