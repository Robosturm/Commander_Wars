#include "objects/dialogs/ingame/victoryrulepopup.h"
#include "objects/base/label.h"

#include "game/gamemap.h"
#include "game/gamerules.h"
#include "game/victoryrule.h"
#include "game/actionperformer.h"

#include "menue/gamemenue.h"

#include "resource_management/fontmanager.h"

QStringList VictoryRulePopup::m_popUps;

VictoryRulePopup::VictoryRulePopup(GameMap* pMap, QString rule, qint32 width, qint32 heigth)
    : CloseablePopUp(width, heigth),
      m_rule(rule),
      m_pMap(pMap)
{
    if (pMap != nullptr)
    {
        GameMenue* pMenu = dynamic_cast<GameMenue*>(pMap->getMenu());
        if (pMenu != nullptr)
        {
            connect(pMenu->getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &VictoryRulePopup::updateInfo, Qt::QueuedConnection);
        }
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
        styleSmall.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        styleSmall.multiline = true;

        spLabel pTextfield = MemoryManagement::create<Label>(getHeight() - 20);
        pTextfield->setStyle(styleSmall);
        pTextfield->setHtmlText(pVictoryRule->getRuleName());
        pTextfield->setPosition(10, y);
        addItem(pTextfield);
        y += 30 + pTextfield->getTextRect().height();

        qint32 width = pTextfield->getTextRect().width();

        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = true;

        qint32 itemCount = pVictoryRule->getRuleTargetCount();


        for (qint32 i2 = 0; i2 < m_pMap->getPlayerCount(); i2++)
        {
            Player* pPlayer = m_pMap->getPlayer(i2);
            if (pPlayer->getIsDefeated() == false)
            {
                for (qint32 item = 0; item < itemCount; ++item)
                {
                    qint32 ruleValue = pVictoryRule->getRuleTargetValue(item, pPlayer);
                    qint32 playerValue = pVictoryRule->getRuleProgress(item, pPlayer);

                    spBuilding building = MemoryManagement::create<Building>("HQ", m_pMap);
                    building->setOwner(pPlayer);
                    building->setPosition(x, y);
                    addItem(building);
                    QString info = QString::number(playerValue) + "/" + QString::number(ruleValue);
                    spLabel pTextfield = MemoryManagement::create<Label>(getWidth() - 20);
                    pTextfield->setStyle(style);
                    pTextfield->setHtmlText(info);
                    pTextfield->setPosition(x + GameMap::getImageSize() + 5, y - 15);
                    if (width < pTextfield->getTextRect().width() + x + 10)
                    {
                        width = pTextfield->getTextRect().width() + x + 10;
                    }
                    addItem(pTextfield);
                    y += GameMap::getImageSize() * 2 + 5;
                }
            }
        }
        setContentHeigth(y);
        setContentWidth(width);
    }
}
