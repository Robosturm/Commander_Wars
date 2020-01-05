#include "dialogvictoryconditions.h"


#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "objects/panel.h"

#include "game/gamemap.h"
#include "game/gamerules.h"
#include "game/victoryrule.h"
#include "game/player.h"
#include "game/co.h"

DialogVictoryConditions::DialogVictoryConditions()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() / 2 - m_OkButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        detach();
        emit sigFinished();
    });

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    GameMap* pMap = GameMap::getInstance();
    GameRules* pRules = pMap->getGameRules();

    qint32 y = 10;
    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Victory Info"));
    pTextfield->setScale(2.0f);
    pTextfield->setPosition(pApp->getSettings()->getWidth() / 2 - pTextfield->getTextRect().getWidth(), y);
    pPanel->addItem(pTextfield);
    y += 50;
    QString info = pMap->getGameScript()->getVictoryInfo();

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(info);
    pTextfield->setWidth(pApp->getSettings()->getWidth() - 60);
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    y += 10 + pTextfield->getTextRect().getHeight();
    for (qint32 i = 0; i < pRules->getVictoryRuleSize(); i++)
    {
        VictoryRule* pVictoryRule = pRules->getVictoryRule(i);
        info = pVictoryRule->getRuleDescription();

        pTextfield = new oxygine::TextField();
        pTextfield->setStyle(style);
        pTextfield->setWidth(pApp->getSettings()->getWidth() - 60);
        pTextfield->setHtmlText(info);
        pTextfield->setPosition(10, y);
        pPanel->addItem(pTextfield);
        y += 30 + pTextfield->getTextRect().getHeight();

        qint32 x = 10;
        qint32 stepWidth = 250;
        for (qint32 i2 = 0; i2 < pMap->getPlayerCount(); i2++)
        {
            Player* pPlayer = pMap->getPlayer(i2);
            if (pPlayer->getIsDefeated() == false)
            {
                qint32 ruleValue = pVictoryRule->getRuleValue(0);
                if (pVictoryRule->getRuleType()[0] == VictoryRule::checkbox)
                {
                    ruleValue = 0;
                }
                qint32 playerValue = pVictoryRule->getRuleProgress(pPlayer);
                info = tr("Player ") + QString::number(i2 + 1) + ": " + QString::number(playerValue) + "/" + QString::number(ruleValue);
                spBuilding building = new Building("HQ");
                // building->setScale(1.5f);
                building->setOwner(pPlayer);
                building->setPosition(x, y);
                pPanel->addItem(building);

                pTextfield = new oxygine::TextField();
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(info);
                pTextfield->setPosition(x + GameMap::getImageSize() + 5, y - 15);
                pPanel->addItem(pTextfield);
                x += stepWidth;
                if (x + stepWidth > pApp->getSettings()->getWidth() - 70 && i2 < pMap->getPlayerCount() - 1)
                {
                    x = 10;
                    y += 60;
                }
            }
        }
        y += 40;
    }
    pPanel->setContentHeigth(y + 40);
}
