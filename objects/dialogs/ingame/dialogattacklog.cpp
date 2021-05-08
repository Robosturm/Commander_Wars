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

DialogAttackLog::DialogAttackLog(Player* pPlayer)
    : QObject(),
      m_pPlayer(pPlayer)
{
    setObjectName("DialogAttackLog");
    spGameMap pMap = GameMap::getInstance();
    m_Log = pMap->getGameRecorder()->getAttackLog(pPlayer->getPlayerID());

    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() / 2 - pOkButton->getWidth() / 2, Settings::getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });

    spPanel pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 150));
    pPanel->setPosition(30, 70);

    pSpriteBox->addChild(pPanel);
    qint32 y = 30;
    spLabel pText = spLabel::create(130);
    pText->setStyle(style);
    pText->setHtmlText("Attacker");
    pText->setPosition(10 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText("X");
    pText->setPosition(140 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText("Y");
    pText->setPosition(240 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(100);
    pText->setStyle(style);
    pText->setHtmlText("Dealt");
    pText->setPosition(330 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(130);
    pText->setStyle(style);
    pText->setHtmlText("Defender");
    pText->setPosition(440 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText("X");
    pText->setPosition(580 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(80);
    pText->setStyle(style);
    pText->setHtmlText("Y");
    pText->setPosition(670 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(100);
    pText->setStyle(style);
    pText->setHtmlText("Dealt");
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
            oxygine::spTextField pText = oxygine::spTextField::create();
            pText->setHtmlText(tr("Day ") + QString::number(currentDay));
            pText->setStyle(style);
            pText->setPosition(10, y);
            pPanel->addItem(pText);
            y += 40;
        }

        Terrain* pTerrain = pMap->getTerrain(log->attackerX, log->attackerY);
        spTerrain pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "");
        pActor->loadSprites();
        Building* pBuilding = pTerrain->getBuilding();
        if (pBuilding != nullptr)
        {
            spBuilding pTerrainBuilding = spBuilding::create(pBuilding->getBuildingID());
            pTerrainBuilding->setOwner(nullptr);
            pTerrainBuilding->scaleAndShowOnSingleTile();
            pActor->addChild(pTerrainBuilding);
        }
        pActor->addChild(spUnit::create(log->attackerID, pMap->getPlayer(log->attackerOwnerID), false));
        pActor->setPosition(60, y + 8);
        if (log->attackerKilled)
        {
            oxygine::spSprite pSprite = oxygine::spSprite::create();
            oxygine::ResAnim* pAnim = pGameManager->getResAnim("fire");
            pSprite->setResAnim(pAnim);
            pSprite->setScale(GameMap::getImageSize() / pAnim->getWidth() * 0.75f);
            pSprite->setPosition(0, GameMap::getImageSize() * 1.0f / 4.0f);
            pActor->addChild(pSprite);
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
            pText->setHtmlText("?");
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
            pText->setHtmlText("?");
        }
        pText->setPosition(230, y);
        pPanel->addItem(pText);

        pText = spLabel::create(100);
        pText->setStyle(style);
        pText->setHtmlText(QString::number(log->attackerDamage) + tr("Hp"));
        pText->setPosition(320, y);
        pPanel->addItem(pText);

        pTerrain = pMap->getTerrain(log->defenderX, log->defenderY);
        pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "");
        pBuilding = pTerrain->getBuilding();
        pActor->loadSprites();
        if (pBuilding != nullptr)
        {
            spBuilding pTerrainBuilding = spBuilding::create(pBuilding->getBuildingID());
            pTerrainBuilding->setOwner(nullptr);
            pTerrainBuilding->scaleAndShowOnSingleTile();
            pActor->addChild(pTerrainBuilding);
        }
        pActor->addChild(spUnit::create(log->defenderID, pMap->getPlayer(log->defenderOwnerID), false));
        if (log->defenderKilled)
        {
            oxygine::spSprite pSprite = oxygine::spSprite::create();
            oxygine::ResAnim* pAnim = pGameManager->getResAnim("fire");
            pSprite->setResAnim(pAnim);
            pSprite->setScale(GameMap::getImageSize() / pAnim->getWidth() * 0.75f);
            pSprite->setPosition(0, GameMap::getImageSize() * 1.0f / 4.0f);
            pActor->addChild(pSprite);
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
            pText->setHtmlText("?");
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
            pText->setHtmlText("?");
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
        pButton->addClickListener([=](oxygine::Event*)
        {
            emit sigShowAttack(posAtkX, posAtkY, playerAtk, posDefX, posDefY, playerDef);
        });
        pPanel->addItem(pButton);
        y += 40;
    }
    pPanel->setContentWidth(1000);
    pPanel->setContentHeigth(y + 40);

    connect(this, &DialogAttackLog::sigShowAttack, this, &DialogAttackLog::showAttack, Qt::QueuedConnection);
}

void DialogAttackLog::showAttack(qint32 posAtkX, qint32 posAtkY, qint32 playerAtk, qint32 posDefX, qint32 posDefY, qint32 playerDef)
{
    
    spGameMap pMap = GameMap::getInstance();
    HumanPlayerInput* pInput = dynamic_cast<HumanPlayerInput*>(pMap->getCurrentPlayer()->getBaseGameInput());
    if (pInput != nullptr)
    {
        pInput->createMarkedField(QPoint(posAtkX, posAtkY), pMap->getPlayer(playerAtk)->getColor(), Terrain::DrawPriority::MarkedFieldMap);
        pInput->createMarkedField(QPoint(posDefX, posDefY), pMap->getPlayer(playerDef)->getColor(), Terrain::DrawPriority::MarkedFieldMap);
    }
    emit sigFinished();
    oxygine::Actor::detach();
}
