#include "victoryrulepopup.h"

#include "game/gamemap.h"
#include "game/gamerules.h"
#include "game/victoryrule.h"
#include "resource_management/fontmanager.h"
#include "menue/gamemenue.h"

#include "objects/base/label.h"

QStringList VictoryRulePopup::m_popUps;

VictoryRulePopup::VictoryRulePopup(QString rule, qint32 width, qint32 heigth)
    : CloseablePopUp(width, heigth),
      m_rule(rule)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        connect(pMenu.get(), &GameMenue::sigActionPerformed, this, &VictoryRulePopup::updateInfo, Qt::QueuedConnection);
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
    spGameMap pMap = GameMap::getInstance();
    clearContent();
    if (pMap.get() != nullptr)
    {
        GameRules* pRules = pMap->getGameRules();
        VictoryRule* pVictoryRule = pRules->getVictoryRule(m_rule);
        qint32 x = 10;
        qint32 y = 10;

        oxygine::TextStyle styleSmall = FontManager::getMainFont16();
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

        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = true;

        for (qint32 i2 = 0; i2 < pMap->getPlayerCount(); i2++)
        {
            Player* pPlayer = pMap->getPlayer(i2);
            if (pPlayer->getIsDefeated() == false)
            {
                qint32 ruleValue = pVictoryRule->getRuleValue(0);
                auto ruleTypes = pVictoryRule->getRuleType();
                if (ruleTypes[0] == VictoryRule::checkbox)
                {
                    ruleValue = 0;
                }
                qint32 playerValue = pVictoryRule->getRuleProgress(pPlayer);
                spBuilding building = spBuilding::create("HQ");
                // building->setScale(1.5f);
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
