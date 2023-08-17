#include "objects/ruleselection.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "coreengine/gameconsole.h"
#include "coreengine/globalutils.h"

#include "game/gamemap.h"

#include "resource_management/gamerulemanager.h"

#include "objects/base/multislider.h"
#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/rules/cobannlistdialog.h"
#include "objects/dialogs/rules/perkselectiondialog.h"
#include "objects/dialogs/rules/actionlistdialog.h"

#include "ui_reader/uifactory.h"

constexpr qint32 textWidth = 300;

RuleSelection::RuleSelection(GameMap* pMap, qint32 width, Mode mode, bool enabled)
    : m_mode(mode),
      m_ruleChangeEabled(enabled),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("RuleSelection");
#endif
    Interpreter::setCppOwnerShip(this);
    setWidth(width);
    connect(this, &RuleSelection::sigShowRuleSelection, this, &RuleSelection::showRuleSelection, Qt::QueuedConnection);

    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    qint32 initCount = m_pMap->getGameRules()->getVictoryRuleSize();
    CONSOLE_PRINT("Creating ruleset number of initial rules " + QString::number(initCount), GameConsole::eDEBUG);
    for (qint32 i = 0; i < pGameRuleManager->getVictoryRuleCount(); i++)
    {
        qint32 xPos = 0;
        QString ruleID = pGameRuleManager->getVictoryRuleID(i);
        spVictoryRule pRule = m_pMap->getGameRules()->getSpVictoryRule(ruleID);
        if (pRule.get() == nullptr && m_ruleChangeEabled)
        {
            CONSOLE_PRINT("Creating default ruleset for " + ruleID, GameConsole::eDEBUG);
            pRule = MemoryManagement::create<VictoryRule>(ruleID, m_pMap);
            QStringList types = pRule->getRuleType();
            for (qint32 i2 = 0; i2 < types.size(); i2++)
            {
                if (types[0] == VictoryRule::checkbox && i2 == 0)
                {
                    if (initCount == 0)
                    {
                        pRule->setRuleValue(pRule->getDefaultValue(0), 0);
                    }
                    else
                    {
                        pRule->setRuleValue(0, 0);
                    }
                }
                else
                {
                    pRule->setRuleValue(pRule->getDefaultValue(i2), i2);
                }
            }
            m_pMap->getGameRules()->addVictoryRule(pRule);
        }
    }
    for (qint32 i = 0; i < pGameRuleManager->getGameRuleCount(); i++)
    {
        qint32 xPos = 0;
        QString ruleID = pGameRuleManager->getGameRuleID(i);
        spGameRule pRule = m_pMap->getGameRules()->getSpGameRule(ruleID);
        if (pRule.get() == nullptr)
        {
            pRule = MemoryManagement::create<GameRule>(ruleID);
            QStringList types = pRule->getRuleType();
            for (qint32 i2 = 0; i2 < types.size(); i2++)
            {
                if (types[0] == VictoryRule::checkbox && i2 == 0)
                {
                    if (initCount == 0)
                    {
                        pRule->setRuleValue(pRule->getDefaultValue(0), 0);
                    }
                    else
                    {
                        pRule->setRuleValue(0, 0);
                    }
                }
                else
                {
                    pRule->setRuleValue(pRule->getDefaultValue(i2), i2);
                }
            }
            m_pMap->getGameRules()->addGameRule(pRule);
        }
    }
    showRuleSelection();
}

bool RuleSelection::getIsMultiplayerMode()
{
    return m_mode == Mode::Multiplayer;
}

bool RuleSelection::getIsEditorMode()
{
    return m_mode == Mode::Editor;
}

void RuleSelection::showRuleSelection()
{
    removeChildren();
    setSize(0, 0);
    UiFactory::getInstance().createUi("ui/game/ruleSelection.xml", this);
    emit sigSizeChanged();
}

bool RuleSelection::getShowAdvanced() const
{
    return m_showAdvanced;
}

void RuleSelection::setShowAdvanced(bool newShowAdvanced)
{
    m_showAdvanced = newShowAdvanced;
    emit sigShowRuleSelection();
}

bool RuleSelection::getRuleChangeEabled() const
{
    return m_ruleChangeEabled;
}

GameMap *RuleSelection::getMap() const
{
    return m_pMap;
}

RuleSelection::Mode RuleSelection::getMode() const
{
    return m_mode;
}

void RuleSelection::startWeatherChanged(qint32 value)
{
    m_pMap->getGameRules()->setStartWeather(value);
}

void RuleSelection::weatherChancesChanged(const QString id)
{
    Multislider* pWeatherSlider = getCastedObject<Multislider>(id);
    for (qint32 i = 0; i < m_pMap->getGameRules()->getWeatherCount(); i++)
    {
        m_pMap->getGameRules()->changeWeatherChance(i, pWeatherSlider->getSliderValue(i));
    }
}

void RuleSelection::showCOBannlist()
{
    spCOBannListDialog pBannlist = MemoryManagement::create<COBannListDialog>(m_pMap->getGameRules()->getCOBannlist());
    oxygine::Stage::getStage()->addChild(pBannlist);
    connect(pBannlist.get(), &COBannListDialog::editFinished, m_pMap->getGameRules(), &GameRules::setCOBannlist, Qt::QueuedConnection);
    
}

void RuleSelection::showPerkBannlist()
{
    spPerkSelectionDialog pBannlist = MemoryManagement::create<PerkSelectionDialog>(m_pMap, nullptr, true, QStringList());
    oxygine::Stage::getStage()->addChild(pBannlist);
    connect(pBannlist.get(), &PerkSelectionDialog::editFinished, m_pMap->getGameRules(), &GameRules::setAllowedPerks, Qt::QueuedConnection);
}

void RuleSelection::showActionBannlist()
{
    spActionListDialog pBannlist = MemoryManagement::create<ActionListDialog>(m_pMap->getGameRules()->getAllowedActions(), m_pMap);
    oxygine::Stage::getStage()->addChild(pBannlist);
    connect(pBannlist.get(), &ActionListDialog::editFinished, m_pMap->getGameRules(), &GameRules::setAllowedActions, Qt::QueuedConnection);
    
}

void RuleSelection::showSelectScript()
{
    QStringList wildcards;
    wildcards.append("*.js");
    QString path = Settings::getInstance()->getUserPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Load"));
    oxygine::Stage::getStage()->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &RuleSelection::scriptFileChanged, Qt::QueuedConnection);
}

void RuleSelection::scriptFileChanged(const QString file)
{
    Textbox* pTextbox = getCastedObject<Textbox>("ScriptTextbox");
    pTextbox->setCurrentText(GlobalUtils::makePathRelative(file));
    m_pMap->getGameScript()->setScriptFile(file);
    m_pMap->getGameScript()->init();    
}
