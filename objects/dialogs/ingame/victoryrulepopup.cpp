#include "objects/dialogs/ingame/victoryrulepopup.h"
#include "objects/base/label.h"

#include "game/gamemap.h"
#include "game/gamerules.h"
#include "game/victoryrule.h"
#include "game/actionperformer.h"

#include "menue/gamemenue.h"
#include "menue/movementplanner.h"

#include "resource_management/fontmanager.h"

QStringList VictoryRulePopup::m_popUps;

VictoryRulePopup::VictoryRulePopup(GameMap* pMap, QString rule, qint32 width, qint32 heigth)
    : CloseablePopUp(width, heigth),
      m_rule(rule),
      m_pMap(pMap)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        connect(&pMenu->getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &VictoryRulePopup::updateInfo, Qt::QueuedConnection);
    }
    updateInfo();
    m_popUps.append(m_rule);
}

VictoryRulePopup::~VictoryRulePopup()
{
    m_popUps.removeAll(m_rule);
}

void VictoryRulePopup::updateInfo()
{
    
    clearContent();
    if (m_pMap != nullptr)
    {
        GameRules* pRules = m_pMap->getGameRules();
        VictoryRule* pVictoryRule = pRules->getVictoryRule(m_rule);
        qint32 x = 10;
        qint32 y = 10;

        oxygine::TextStyle styleSmall = oxygine::TextStyle(FontManager::getMainFont16());
        styleSmall.color = FontManager::getFontColor();
        styleSmall.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        styleSmall.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        styleSmall.multiline = true;

        spLabel pTextfield = spLabel::create(getHeight() - 20);
        pTextfield->setStyle(styleSmall);
        pTextfield->setHtmlText(pVictoryRule->getRuleName());
        pTextfield->setPosition(10, y);
        addItem(pTextfield);
        y += 30 + pTextfield->getTextRect().getHeight();

        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = true;

        for (qint32 i2 = 0; i2 < m_pMap->getPlayerCount(); i2++)
        {
            Player* pPlayer = m_pMap->getPlayer(i2);
            if (pPlayer->getIsDefeated() == false)
            {
                qint32 ruleValue = pVictoryRule->getRuleValue(0);
                auto ruleTypes = pVictoryRule->getRuleType();
                if (ruleTypes[0] == VictoryRule::checkbox)
                {
                    ruleValue = 0;
                }
                qint32 playerValue = pVictoryRule->getRuleProgress(pPlayer);
                spBuilding building = spBuilding::create("HQ", m_pMap);
                building->setOwner(pPlayer);
                building->setPosition(x, y);
                addItem(building);
                QString info = QString::number(playerValue) + "/" + QString::number(ruleValue);
                spLabel pTextfield = spLabel::create(getWidth() - 20);
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(info);
                pTextfield->setPosition(x + GameMap::getImageSize() + 5, y - 15);
                addItem(pTextfield);
                y += GameMap::getImageSize() * 2 + 5;
            }
        }
        setContentHeigth(y);
    }
}
