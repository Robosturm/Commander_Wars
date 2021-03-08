#include "ruleselection.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "game/gamemap.h"

#include "resource_management/gamerulemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/timespinbox.h"
#include "objects/dialogs/rules/cobannlistdialog.h"
#include "objects/dialogs/rules/perkselectiondialog.h"
#include "objects/dialogs/rules/actionlistdialog.h"
#include "objects/base/label.h"
#include "objects/dialogs/filedialog.h"
#include "objects/base/passwordbox.h"
#include "objects/base/textbox.h"

constexpr qint32 textWidth = 300;

RuleSelection::RuleSelection(qint32 width, Mode mode, bool enabled)
    : QObject(),
      m_mode(mode),
      m_ruleChangeEabled(enabled)
{
    setWidth(width);
    showRuleSelection();
}

RuleSelection::~RuleSelection()
{
}

void RuleSelection::confirmRuleSelectionSetup()
{
    if (m_ruleChangeEabled)
    {
        Console::print("Confirming rule selection and enabling/disabling rules for the map.", Console::eDEBUG);
        GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
        spGameMap pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pGameRuleManager->getVictoryRuleCount(); i++)
        {
            QString ruleID = pGameRuleManager->getVictoryRuleID(i);
            spVictoryRule pRule = pMap->getGameRules()->getVictoryRule(ruleID);
            if (pRule.get() != nullptr)
            {
                QStringList inputTypes = pRule->getRuleType();
                if (inputTypes[0] == VictoryRule::checkbox)
                {
                    qint32 ruleValue = pRule->getRuleValue(0);
                    if (ruleValue == 0)
                    {
                        Console::print("Removing rule cause it's disabled: " + ruleID, Console::eDEBUG);
                        pMap->getGameRules()->removeVictoryRule(ruleID);
                    }
                    else
                    {
                        Console::print("Rule is enabled: " + ruleID, Console::eDEBUG);
                    }
                }
                else if (inputTypes[0] == VictoryRule::spinbox)
                {
                    qint32 ruleValue = pRule->getRuleValue(0);
                    qint32 infiniteValue = pRule->getInfiniteValue(0);
                    if (ruleValue <= infiniteValue)
                    {
                        Console::print("Removing rule cause it's disabled: " + ruleID, Console::eDEBUG);
                        pMap->getGameRules()->removeVictoryRule(ruleID);
                    }
                    else
                    {
                        Console::print("Rule is enabled: " + ruleID, Console::eDEBUG);
                    }
                }
                else
                {
                    Console::print("Removing rule cause it's in unsupported format: " + ruleID, Console::eERROR);
                    pMap->getGameRules()->removeVictoryRule(ruleID);
                }
            }
        }

        for (qint32 i = 0; i < pGameRuleManager->getGameRuleCount(); i++)
        {
            QString ruleID = pGameRuleManager->getGameRuleID(i);
            spGameRule pRule = pMap->getGameRules()->getGameRule(ruleID);
            if (pRule.get() != nullptr)
            {
                QStringList inputTypes = pRule->getRuleType();
                if (inputTypes[0] == VictoryRule::checkbox)
                {
                    qint32 ruleValue = pRule->getRuleValue(0);
                    if (ruleValue == 0)
                    {
                        Console::print("Removing rule cause it's disabled: " + ruleID, Console::eDEBUG);
                        pMap->getGameRules()->removeGameRule(ruleID);
                    }
                    else
                    {
                        Console::print("Rule is enabled: " + ruleID, Console::eDEBUG);
                    }
                }
                else if (inputTypes[0] == VictoryRule::spinbox)
                {
                    qint32 ruleValue = pRule->getRuleValue(0);
                    qint32 infiniteValue = pRule->getInfiniteValue(0);
                    if (ruleValue <= infiniteValue)
                    {
                        Console::print("Removing rule cause it's disabled: " + ruleID, Console::eDEBUG);
                        pMap->getGameRules()->removeGameRule(ruleID);
                    }
                    else
                    {
                        Console::print("Rule is enabled: " + ruleID, Console::eDEBUG);
                    }
                }
                else
                {
                    Console::print("Removing rule cause it's in unsupported format: " + ruleID, Console::eERROR);
                    pMap->getGameRules()->removeGameRule(ruleID);
                }
            }
        }
    }
}

void RuleSelection::showRuleSelection()
{
    
    removeChildren();
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    qint32 y = 20;
    // font style
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    QColor headerColor(0, 255, 0, 255);
    spGameMap pMap = GameMap::getInstance();

    if (m_mode == Mode::Multiplayer)
    {
        spLabel textField = new Label(textWidth - 40);
        textField->setStyle(style);
        textField->setHtmlText(tr("Game Description: "));
        textField->setPosition(30, y);
        addChild(textField);
        spTextbox pTexbox = new Textbox(Settings::getWidth() - 100 - textWidth);
        pTexbox->setPosition(textWidth, y);
        pTexbox->setCurrentText("");
        pTexbox->setTooltipText(tr("Map description shown for players who want to join. Keep it short here."));
        pTexbox->setEnabled(m_ruleChangeEabled);
        connect(pTexbox.get(), &Textbox::sigTextChanged, pMap->getGameRules(), &GameRules::setDescription, Qt::QueuedConnection);
        addChild(pTexbox);
        y += 40;

        textField = new Label(textWidth - 40);
        textField->setStyle(style);
        textField->setHtmlText(tr("Password: "));
        textField->setPosition(30, y);
        addChild(textField);

        spPasswordbox pPasswordbox = new Passwordbox(Settings::getWidth() - 100 - textWidth);
        pPasswordbox->setPosition(textWidth, y);
        pPasswordbox->setCurrentText("");
        pPasswordbox->setTooltipText(tr("Map description shown for players who want to join. Keep it short here."));
        pPasswordbox->setEnabled(m_ruleChangeEabled);
        connect(pPasswordbox.get(), &Passwordbox::sigTextChanged, pMap->getGameRules(), &GameRules::setPassword, Qt::QueuedConnection);
        addChild(pPasswordbox);
        y += 40;

        textField = new Label(textWidth - 40);
        textField->setStyle(style);
        textField->setHtmlText(tr("Cosmetic Mods: "));
        textField->setPosition(30, y);
        addChild(textField);
        spCheckbox pCheckbox = new Checkbox();
        pCheckbox->setPosition(textWidth, y);
        pCheckbox->setChecked(false);
        pCheckbox->setTooltipText(tr("If checked cosmetic mods can be different on host and client site.\nWarning this may lead to asynchron games or crashes in case one of the mods is not a pure cosmetic mod."));
        pCheckbox->setEnabled(m_ruleChangeEabled);
        connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setCosmeticModsAllowed, Qt::QueuedConnection);
        addChild(pCheckbox);
        y += 40;
    }

    spLabel textField = new Label(800);
    style.color = headerColor;
    textField->setStyle(headerStyle);
    style.color = FontManager::getFontColor();
    textField->setHtmlText(tr("Environment"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 60;


    QVector<QString> weatherStrings;
    QVector<qint32> weatherChances;
    for (qint32 i = 0; i < pMap->getGameRules()->getWeatherCount(); i++)
    {
        Weather* pWeather = pMap->getGameRules()->getWeather(i);
        weatherStrings.append(pWeather->getWeatherName());
        weatherChances.append(pMap->getGameRules()->getWeatherChance(pWeather->getWeatherId()));
    }
    m_pWeatherSlider = new Multislider(weatherStrings, static_cast<qint32>(getWidth()), weatherChances);
    m_pWeatherSlider->setTooltipText(tr("The chance each weather can appear if random weather is active."));
    m_pWeatherSlider->setPosition(30, y);
    m_pWeatherSlider->setEnabled(m_ruleChangeEabled);
    addChild(m_pWeatherSlider);
    connect(m_pWeatherSlider.get(), &Multislider::signalSliderChanged, this, &RuleSelection::weatherChancesChanged, Qt::QueuedConnection);

    y += m_pWeatherSlider->getHeight();
    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Random Weather: "));
    textField->setPosition(30, y);
    addChild(textField);
    spCheckbox pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked random weather appears during the game."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getRandomWeather());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setRandomWeather, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Start Weather: "));
    textField->setPosition(30, pCheckbox->getY() + 40);
    addChild(textField);

    spDropDownmenu startWeather = new DropDownmenu(300, weatherStrings);
    startWeather->setTooltipText(tr("The weather at the start of the game."));
    startWeather->setPosition(textWidth, textField->getY());
    startWeather->setCurrentItem(pMap->getGameRules()->getStartWeather());
    startWeather->setEnabled(m_ruleChangeEabled);
    connect(startWeather.get(), &DropDownmenu::sigItemChanged, this, &RuleSelection::startWeatherChanged, Qt::QueuedConnection);
    addChild(startWeather);
    startWeatherChanged(pMap->getGameRules()->getStartWeather());
    y += 50;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Weather Forecast: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked you can see the weather for the next few turns for each player."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getWeatherPrediction());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setWeatherPrediction, Qt::QueuedConnection);

    y = textField->getY() + 50;
    textField = new Label(800);
    style.color = headerColor;
    textField->setStyle(headerStyle);
    style.color = FontManager::getFontColor();
    textField->setHtmlText(tr("Gameplay"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 60;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Unit Ranking: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked units can gain ranks by killing other units."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getRankingSystem());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setRankingSystem, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("No CO Powers: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked CO's can't use CO-Powers."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getNoPower());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setNoPower, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Single CO's: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked you can only select a single co for a player."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getSingleCo());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setSingleCo, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("CO Specific Units: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If unchecked specific CO-Units can't be produced."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getCoUnits());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setCoUnits, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("CO Perks: "));
    textField->setPosition(30, y);
    addChild(textField);
    spSpinBox pSpinbox = new SpinBox(300, 0, 900);
    pSpinbox->setTooltipText(tr("Selects the amount of CO Perks that can be assigned per CO."));
    pSpinbox->setPosition(textWidth, textField->getY());
    pSpinbox->setInfinityValue(-1);
    pSpinbox->setEnabled(m_ruleChangeEabled);
    addChild(pSpinbox);
    pSpinbox->setCurrentValue(pMap->getGameRules()->getMaxPerkCount());
    connect(pSpinbox.get(), &SpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setMaxPerkCount, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Unit Limit: "));
    textField->setPosition(30, y);
    addChild(textField);
    pSpinbox = new SpinBox(200, 0, 9999);
    pSpinbox->setTooltipText(tr("The maximum amount of units a single player can own at any time."));
    pSpinbox->setInfinityValue(0.0);
    pSpinbox->setPosition(textWidth, textField->getY());
    pSpinbox->setEnabled(m_ruleChangeEabled);
    addChild(pSpinbox);
    pSpinbox->setCurrentValue(pMap->getGameRules()->getUnitLimit());
    connect(pSpinbox.get(), &SpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setUnitLimit, Qt::QueuedConnection);
    y += 40;

    textField = new Label(800);
    style.color = headerColor;
    textField->setStyle(headerStyle);
    style.color = FontManager::getFontColor();
    textField->setHtmlText(tr("Fog of War"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 60;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Fog Of War: "));
    textField->setPosition(30, y);
    addChild(textField);
    QVector<QString> fogModes = {tr("Off"), tr("Fog of War"), tr("Shroud of War")};
    spDropDownmenu fogOfWar = new DropDownmenu(300, fogModes);
    fogOfWar->setTooltipText(tr("Select the fog of war rule for the current game."));
    fogOfWar->setPosition(textWidth, textField->getY());
    fogOfWar->setCurrentItem(pMap->getGameRules()->getFogMode());
    fogOfWar->setEnabled(m_ruleChangeEabled);
    connect(fogOfWar.get(), &DropDownmenu::sigItemChanged, pMap->getGameRules(), [=](qint32 value)
    {
        pMap->getGameRules()->setFogMode(static_cast<GameEnums::Fog>(value));
    });
    addChild(fogOfWar);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Vision Block: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked units can't see over certain terrains. Reducing their vision range. Air units are unaffected by this effect."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getVisionBlock());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setVisionBlock, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Building Hidding:"));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked most buildings deny vision. E.g. you can hide a unit in a building similar to a forest."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getBuildingVisionHide());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setBuildingVisionHide, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Day Screen: "));
    textField->setPosition(30, y);
    addChild(textField);
    QVector<QString> dayModes = {tr("Default"), tr("Permanent")};
    spDropDownmenu pDropDownmenu = new DropDownmenu(300, dayModes);
    pDropDownmenu->setTooltipText(tr("Defines if the day to day banner is shown permanent for human or not. Decision is depending of chosen fog of war."));
    pDropDownmenu->setPosition(textWidth, textField->getY());
    pDropDownmenu->setEnabled(m_ruleChangeEabled);
    addChild(pDropDownmenu);
    pDropDownmenu->setCurrentItem(static_cast<qint32>(pMap->getGameRules()->getDayToDayScreen()));
    connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, [=](qint32 item)
    {
        pMap->getGameRules()->setDayToDayScreen(static_cast<GameRules::DayToDayScreen>(item));
    });
    y += 50;

    textField = new Label(800);
    style.color = headerColor;
    textField->setStyle(headerStyle);
    style.color = FontManager::getFontColor();
    textField->setHtmlText(tr("Advanced"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 60;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("CO Banlist: "));
    textField->setPosition(30, y);
    addChild(textField);
    oxygine::spButton coBannlist = ObjectManager::createButton(tr("Edit"), 150);
    coBannlist->setPosition(textWidth, y - 2);
    coBannlist->setEnabled(m_ruleChangeEabled);
    coBannlist->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowCOBannlist();
    });
    addChild(coBannlist);
    connect(this, &RuleSelection::sigShowCOBannlist, this, &RuleSelection::showCOBannlist, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Perk Banlist: "));
    textField->setPosition(30, y);
    addChild(textField);
    oxygine::spButton perkBannlist = ObjectManager::createButton(tr("Edit"), 150);
    perkBannlist->setPosition(textWidth, y - 2);
    perkBannlist->setEnabled(m_ruleChangeEabled);
    perkBannlist->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowPerkBannlist();
    });
    addChild(perkBannlist);
    connect(this, &RuleSelection::sigShowPerkBannlist, this, &RuleSelection::showPerkBannlist, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Action Banlist: "));
    textField->setPosition(30, y);
    addChild(textField);
    oxygine::spButton actionBannlist = ObjectManager::createButton(tr("Edit"), 150);
    actionBannlist->setPosition(textWidth, y - 2);
    actionBannlist->setEnabled(m_ruleChangeEabled);
    actionBannlist->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowActionBannlist();
    });
    addChild(actionBannlist);
    connect(this, &RuleSelection::sigShowActionBannlist, this, &RuleSelection::showActionBannlist, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Powergain Speed: "));
    textField->setPosition(30, y);
    addChild(textField);
    pSpinbox = new SpinBox(200, 0, 200, SpinBox::Mode::Float);
    pSpinbox->setTooltipText(tr("The speed at which the power bar fills up. This affects gains from HP damage or Funds damage based on the mode."));
    pSpinbox->setInfinityValue(-1.0);
    pSpinbox->setSpinSpeed(0.1f);
    pSpinbox->setPosition(textWidth, textField->getY());
    pSpinbox->setEnabled(m_ruleChangeEabled);
    addChild(pSpinbox);
    pSpinbox->setCurrentValue(pMap->getGameRules()->getPowerGainSpeed());
    connect(pSpinbox.get(), &SpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setPowerGainSpeed, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Round Time: "));
    textField->setPosition(30, y);
    addChild(textField);
    spTimeSpinBox pTimeSpinbox = new TimeSpinBox(200);
    pTimeSpinbox->setTooltipText(tr("The maximum amount of time in hh:mm::ss for each turn for each player."));
    pTimeSpinbox->setPosition(textWidth, textField->getY());
    pTimeSpinbox->setEnabled(m_ruleChangeEabled);
    addChild(pTimeSpinbox);
    pTimeSpinbox->setCurrentValue(pMap->getGameRules()->getRoundTimeMs());
    connect(pTimeSpinbox.get(), &TimeSpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setRoundTimeMs, Qt::QueuedConnection);
    y += 40;

    // Label
    if (m_mode != RuleSelection::Mode::Editor && m_ruleChangeEabled)
    {
        textField = new Label(textWidth - 10);
        textField->setStyle(style);
        textField->setHtmlText(tr("Script:"));
        textField->setPosition(30, y);
        addChild(textField);
        oxygine::spButton pScriptButton = pObjectManager->createButton(tr("Select File"), 160);
        pScriptButton->setPosition(Settings::getWidth() - pScriptButton->getWidth() - 100, y);
        addChild(pScriptButton);
        m_MapScriptFile = new Textbox(pScriptButton->getX() - textField->getX() - textWidth);
        m_MapScriptFile->setTooltipText(tr("The relative path from the exe to the script associated with this map."));
        m_MapScriptFile->setPosition(textWidth, textField->getY());
        m_MapScriptFile->setCurrentText(pMap->getGameScript()->getScriptFile());
        m_MapScriptFile->setEnabled(m_ruleChangeEabled);
        addChild(m_MapScriptFile);
        pScriptButton->setEnabled(m_ruleChangeEabled);
        pScriptButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectScript();
        });
        connect(this, &RuleSelection::sigShowSelectScript, this, &RuleSelection::showSelectScript, Qt::QueuedConnection);
        y += 40;
    }

    textField = new Label(800);
    style.color = headerColor;
    textField->setStyle(headerStyle);
    style.color = FontManager::getFontColor();
    textField->setHtmlText(tr("Miscellaneous"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 60;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Unique CO's: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked CO's that are randomly selected are unique. Note: If not enough CO's are available this may select no co for a player"));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getSingleRandomCO());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setSingleRandomCO, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("AI Attack Terrain: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked the AI attacks pipe seams and walls etc."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getAiAttackTerrain());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setAiAttackTerrain, Qt::QueuedConnection);
    y += 40;

    textField = new Label(textWidth - 40);
    textField->setStyle(style);
    textField->setHtmlText(tr("Team Facing :"));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked units of the same team have the same direction rather than based on player order."));
    pCheckbox->setPosition(textWidth, textField->getY());
    pCheckbox->setEnabled(m_ruleChangeEabled);
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getTeamFacingUnits());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setTeamFacingUnits, Qt::QueuedConnection);
    y += 50;
    addCustomGamerules(y);

    textField = new Label(800);
    style.color = headerColor;
    textField->setStyle(headerStyle);
    style.color = FontManager::getFontColor();
    textField->setHtmlText(tr("Victory Rules"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 60;

    qint32 initCount = pMap->getGameRules()->getVictoryRuleSize();
    for (qint32 i = 0; i < pGameRuleManager->getVictoryRuleCount(); i++)
    {
        qint32 xPos = 0;
        QString ruleID = pGameRuleManager->getVictoryRuleID(i);
        spVictoryRule pRule = pMap->getGameRules()->getVictoryRule(ruleID);
        if (pRule.get() == nullptr && m_ruleChangeEabled)
        {
            pRule = new VictoryRule(ruleID);
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
            pMap->getGameRules()->addVictoryRule(pRule);
        }
        if (pRule.get() != nullptr)
        {
            QStringList inputTypes = pRule->getRuleType();
            for (qint32 i2 = 0; i2 < inputTypes.size(); i2++)
            {
                QString inputType = inputTypes[i2];
                QString descriptiopn = pRule->getRuleDescription(i2);
                // add a cool check box and a cool text
                QString labelName = pRule->getRuleName(i2);
                textField = new Label(textWidth - 40);
                textField->setStyle(style);
                textField->setHtmlText(labelName);
                textField->setPosition(xPos + 30, y);
                addChild(textField);
                if (inputType == VictoryRule::checkbox)
                {
                    bool defaultValue = pRule->getRuleValue(i2);
                    spCheckbox pCheckbox = new Checkbox();
                    pCheckbox->setTooltipText(descriptiopn);
                    pCheckbox->setPosition(xPos + textWidth, textField->getY());
                    pCheckbox->setEnabled(m_ruleChangeEabled);
                    addChild(pCheckbox);
                    pCheckbox->setChecked(defaultValue);
                    connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
                    {
                        pRule->setRuleValue(value, i2);
                    });
                }
                else if (inputType == VictoryRule::spinbox)
                {
                    qint32 defaultValue = pRule->getRuleValue(i2);
                    qint32 startValue = pRule->getInfiniteValue(i2);
                    spSpinBox pSpinbox = new SpinBox(200, startValue, 9999);
                    pSpinbox->setTooltipText(descriptiopn);
                    pSpinbox->setPosition(xPos + textWidth, textField->getY());
                    pSpinbox->setInfinityValue(startValue);
                    pSpinbox->setEnabled(m_ruleChangeEabled);
                    addChild(pSpinbox);
                    pSpinbox->setCurrentValue(defaultValue);
                    connect(pSpinbox.get(), &SpinBox::sigValueChanged, [=](float value)
                    {
                        qint32 newValue = static_cast<qint32>(value);
                        pRule->setRuleValue(newValue, i2);
                    });
                }
                xPos += textWidth * 2 * 0.85;
                if (xPos + textWidth * 2 * 0.85 + 80 > getWidth())
                {
                    setWidth(xPos + textWidth * 2 * 0.85 + 80);
                }
            }
            y += 50;
        }
    }
    setHeight(y + 50);
}

void RuleSelection::addCustomGamerules(qint32 & y)
{
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    spGameMap pMap = GameMap::getInstance();
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    qint32 initCount = pMap->getGameRules()->getGameRuleSize();
    for (qint32 i = 0; i < pGameRuleManager->getGameRuleCount(); i++)
    {
        qint32 xPos = 0;
        QString ruleID = pGameRuleManager->getGameRuleID(i);
        spGameRule pRule = pMap->getGameRules()->getGameRule(ruleID);
        if (pRule.get() == nullptr)
        {
            pRule = new GameRule(ruleID);
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
        }
        QStringList inputTypes = pRule->getRuleType();
        pMap->getGameRules()->addGameRule(pRule);
        for (qint32 i2 = 0; i2 < inputTypes.size(); i2++)
        {
            QString inputType = inputTypes[i2];
            QString descriptiopn = pRule->getRuleDescription(i2);
            // add a cool check box and a cool text
            QString labelName = pRule->getRuleName(i2);
            spLabel textField = new Label(textWidth - 40);
            textField->setStyle(style);
            textField->setHtmlText(labelName);
            textField->setPosition(xPos + 30, i * 50 + y);
            addChild(textField);
            if (inputType == GameRule::checkbox)
            {
                bool defaultValue = pRule->getRuleValue(i2);
                spCheckbox pCheckbox = new Checkbox();
                pCheckbox->setTooltipText(descriptiopn);
                pCheckbox->setPosition(xPos + textWidth, textField->getY());
                pCheckbox->setEnabled(m_ruleChangeEabled);
                addChild(pCheckbox);
                pCheckbox->setChecked(defaultValue);
                connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
                {
                    pRule->setRuleValue(value, i2);
                });
            }
            else if (inputType == GameRule::spinbox)
            {
                qint32 defaultValue = pRule->getRuleValue(i2);
                qint32 startValue = pRule->getInfiniteValue(i2);
                spSpinBox pSpinbox = new SpinBox(200, startValue, 9999);
                pSpinbox->setTooltipText(descriptiopn);
                pSpinbox->setPosition(xPos + textWidth, textField->getY());
                pSpinbox->setInfinityValue(startValue);
                pSpinbox->setEnabled(m_ruleChangeEabled);
                addChild(pSpinbox);
                pSpinbox->setCurrentValue(defaultValue);
                connect(pSpinbox.get(), &SpinBox::sigValueChanged, [=](float value)
                {
                    qint32 newValue = static_cast<qint32>(value);
                    pRule->setRuleValue(newValue, i2);
                });
            }
            xPos += textWidth * 2 * 0.85;
            if (xPos + textWidth * 2 * 0.85 + 80 > getWidth())
            {
                setWidth(xPos + textWidth * 2 * 0.85 + 80);
            }
        }
    }
    y += pGameRuleManager->getGameRuleCount() * 50;
}

RuleSelection::Mode RuleSelection::getMode() const
{
    return m_mode;
}

void RuleSelection::startWeatherChanged(qint32 value)
{
    GameMap::getInstance()->getGameRules()->setStartWeather(value);
}

void RuleSelection::weatherChancesChanged()
{
    for (qint32 i = 0; i < GameMap::getInstance()->getGameRules()->getWeatherCount(); i++)
    {
        GameMap::getInstance()->getGameRules()->changeWeatherChance(i, m_pWeatherSlider->getSliderValue(i));
    }
}

void RuleSelection::showCOBannlist()
{
    
    spGameMap pMap = GameMap::getInstance();
    spCOBannListDialog pBannlist = new COBannListDialog(pMap->getGameRules()->getCOBannlist());
    oxygine::getStage()->addChild(pBannlist);
    connect(pBannlist.get(), &COBannListDialog::editFinished, pMap->getGameRules(), &GameRules::setCOBannlist, Qt::QueuedConnection);
    
}

void RuleSelection::showPerkBannlist()
{
    spGameMap pMap = GameMap::getInstance();
    spPerkSelectionDialog pBannlist = new PerkSelectionDialog(nullptr, -1, true, QStringList());
    oxygine::getStage()->addChild(pBannlist);
    connect(pBannlist.get(), &PerkSelectionDialog::editFinished, pMap->getGameRules(), &GameRules::setAllowedPerks, Qt::QueuedConnection);
}

void RuleSelection::showActionBannlist()
{
    spGameMap pMap = GameMap::getInstance();
    spActionListDialog pBannlist = new ActionListDialog(pMap->getGameRules()->getAllowedActions());
    oxygine::getStage()->addChild(pBannlist);
    connect(pBannlist.get(), &ActionListDialog::editFinished, pMap->getGameRules(), &GameRules::setAllowedActions, Qt::QueuedConnection);
    
}

void RuleSelection::showSelectScript()
{
    QVector<QString> wildcards;
    wildcards.append("*.js");
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    spFileDialog fileDialog = new FileDialog(path, wildcards);
    oxygine::getStage()->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &RuleSelection::scriptFileChanged, Qt::QueuedConnection);
    
}

void RuleSelection::scriptFileChanged(QString file)
{
    file = file.replace(QCoreApplication::applicationDirPath() + "/", "");
    file = file.replace(QCoreApplication::applicationDirPath(), "");
    m_MapScriptFile->setCurrentText(file);
    spGameMap pMap = GameMap::getInstance();
    pMap->getGameScript()->setScriptFile(file);
    pMap->getGameScript()->init();
    
}
