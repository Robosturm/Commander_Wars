#include "ruleselection.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

#include "resource_management/gamerulemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "objects/checkbox.h"
#include "objects/spinbox.h"
#include "objects/dropdownmenu.h"
#include "objects/timespinbox.h"
#include "objects/cobannlistdialog.h"


RuleSelection::RuleSelection(qint32 width)
    : QObject()
{
    setWidth(width);
    showRuleSelection();
}

RuleSelection::~RuleSelection()
{
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pGameRuleManager->getVictoryRuleCount(); i++)
    {
        QString ruleID = pGameRuleManager->getVictoryRuleID(i);
        spVictoryRule pRule = pMap->getGameRules()->getVictoryRule(ruleID);
        QStringList inputTypes = pRule->getRuleType();
        if (inputTypes[0] == VictoryRule::checkbox)
        {
            if (pRule->getRuleValue(0) == 0)
            {
                pMap->getGameRules()->removeVictoryRule(ruleID);
            }
        }
        else if (inputTypes[0] == VictoryRule::spinbox)
        {
            if (pRule->getRuleValue(0) == pRule->getInfiniteValue(0))
            {
                pMap->getGameRules()->removeVictoryRule(ruleID);
            }
        }
    }
}

void RuleSelection::showRuleSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    removeChildren();
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    qint32 y = 20;
    // font style
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    QColor headerColor(0, 255, 0, 255);
    float headerScale = 1.5f;

    oxygine::spTextField textField = new oxygine::TextField();
    style.color = headerColor;
    textField->setStyle(style);
    style.color = QColor(255, 255, 255, 255);
    textField->setScale(headerScale);
    textField->setHtmlText(tr("Enviroment"));
    textField->setPosition(30, y);    
    addChild(textField);
    y += 40 * headerScale;

    qint32 textWidth = 300;

    QVector<QString> weatherStrings;
    QVector<qint32> weatherChances;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getGameRules()->getWeatherCount(); i++)
    {
        Weather* pWeather = pMap->getGameRules()->getWeather(i);
        weatherStrings.append(pWeather->getWeatherName());
        weatherChances.append(pMap->getGameRules()->getWeatherChance(pWeather->getWeatherId()));
    }
    m_pWeatherSlider = new Multislider(weatherStrings, static_cast<qint32>(getWidth()), weatherChances);
    m_pWeatherSlider->setTooltipText(tr("The chance each weather can appear if random weather is active."));
    m_pWeatherSlider->setPosition(30, y);
    addChild(m_pWeatherSlider);
    connect(m_pWeatherSlider.get(), &Multislider::signalSliderChanged, this, &RuleSelection::weatherChancesChanged, Qt::QueuedConnection);

    y += m_pWeatherSlider->getHeight();
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Random Weather: "));
    textField->setPosition(30, y);
    addChild(textField);
    spCheckbox pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked random weather appears during the game."));
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getRandomWeather());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setRandomWeather, Qt::QueuedConnection);
    y += 40;

    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Start Weather: "));
    textField->setPosition(30, pCheckbox->getY() + 40);
    addChild(textField);

    spDropDownmenu startWeather = new DropDownmenu(200, weatherStrings);
    startWeather->setTooltipText(tr("The weather at the start of the game."));
    startWeather->setPosition(textWidth, textField->getY());
    startWeather->setCurrentItem(pMap->getGameRules()->getStartWeather());
    connect(startWeather.get(), &DropDownmenu::sigItemChanged, this, &RuleSelection::startWeatherChanged, Qt::QueuedConnection);
    addChild(startWeather);
    startWeatherChanged(0);
    y += 50;

    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Weather Forecast: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked you can see the weather for the next few turns for each player."));
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getWeatherPrediction());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setWeatherPrediction, Qt::QueuedConnection);

    y = textField->getY() + 50;
    textField = new oxygine::TextField();
    style.color = headerColor;
    textField->setStyle(style);
    style.color = QColor(255, 255, 255, 255);
    textField->setScale(headerScale);
    textField->setHtmlText(tr("Gameplay"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 40 * headerScale;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Unit Ranking System: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked units can gain ranks by killing other units."));
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getRankingSystem());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setRankingSystem, Qt::QueuedConnection);

    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("CO Bannlist: "));
    textField->setPosition(30, y);
    addChild(textField);
    oxygine::spButton coBannlist = ObjectManager::createButton(tr("Edit"), 150);
    coBannlist->setPosition(textWidth, y - 2);
    coBannlist->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowCOBannlist();
    });
    addChild(coBannlist);
    connect(this, &RuleSelection::sigShowCOBannlist, this, &RuleSelection::showCOBannlist, Qt::QueuedConnection);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("No CO Powers: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked CO's can't use CO-Powers."));
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getNoPower());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setNoPower, Qt::QueuedConnection);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("AI Attack Terrain: "));
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked the AI attacks pipe seams and walls etc."));
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getAiAttackTerrain());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setAiAttackTerrain, Qt::QueuedConnection);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Fog Of War: "));
    textField->setPosition(30, y);
    addChild(textField);
    QVector<QString> fogModes = {tr("Off"), tr("Fog of War")};
    spDropDownmenu fogOfWar = new DropDownmenu(200, fogModes);
    fogOfWar->setTooltipText(tr("Select the fog of war rule for the current game."));
    fogOfWar->setPosition(textWidth, textField->getY());
    fogOfWar->setCurrentItem(pMap->getGameRules()->getFogMode());
    connect(fogOfWar.get(), &DropDownmenu::sigItemChanged, pMap->getGameRules(), [=](qint32 value)
    {
        pMap->getGameRules()->setFogMode(static_cast<GameEnums::Fog>(value));
    });
    addChild(fogOfWar);
    y += 50;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Unit Limit: "));
    textField->setPosition(30, y);
    addChild(textField);
    spSpinBox pSpinbox = new SpinBox(150, 0, 9999);
    pSpinbox->setTooltipText(tr("The maximum amount of units a single player can own at any time."));
    pSpinbox->setInfinityValue(0.0);
    pSpinbox->setPosition(textWidth, textField->getY());
    addChild(pSpinbox);
    pSpinbox->setCurrentValue(pMap->getGameRules()->getUnitLimit());
    connect(pSpinbox.get(), &SpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setUnitLimit, Qt::QueuedConnection);

    y += 50;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Round Time: "));
    textField->setPosition(30, y);
    addChild(textField);
    spTimeSpinBox pTimeSpinbox = new TimeSpinBox(150);
    pTimeSpinbox->setTooltipText(tr("The maximum amount of time in hh:mm::ss for each turn for each player."));
    pTimeSpinbox->setPosition(textWidth, textField->getY());
    addChild(pTimeSpinbox);
    pTimeSpinbox->setCurrentValue(pMap->getGameRules()->getRoundTimeMs());
    connect(pTimeSpinbox.get(), &TimeSpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setRoundTimeMs, Qt::QueuedConnection);


    y += 50;
    textField = new oxygine::TextField();
    style.color = headerColor;
    textField->setStyle(style);
    style.color = QColor(255, 255, 255, 255);
    textField->setScale(headerScale);
    textField->setHtmlText(tr("Victory Rules"));
    textField->setPosition(30, y);
    addChild(textField);
    y += 40 * headerScale;    

    qint32 initCount = pMap->getGameRules()->getVictoryRuleSize();

    for (qint32 i = 0; i < pGameRuleManager->getVictoryRuleCount(); i++)
    {
        qint32 xPos = 0;
        QString ruleID = pGameRuleManager->getVictoryRuleID(i);
        spVictoryRule pRule = pMap->getGameRules()->getVictoryRule(ruleID);
        if (pRule.get() == nullptr)
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
        }
        QStringList inputTypes = pRule->getRuleType();
        pMap->getGameRules()->addVictoryRule(pRule);
        for (qint32 i2 = 0; i2 < inputTypes.size(); i2++)
        {
            QString inputType = inputTypes[i2];
            QString descriptiopn = pRule->getRuleDescription(i2);
            // add a cool check box and a cool text
            QString labelName = pRule->getRuleName(i2);
            textField = new oxygine::TextField();
            textField->setStyle(style);
            textField->setHtmlText(labelName);
            textField->setPosition(xPos + 30, i * 50 + y);
            addChild(textField);
            if (inputType == VictoryRule::checkbox)
            {
                bool defaultValue = pRule->getRuleValue(i2);
                spCheckbox pCheckbox = new Checkbox();
                pCheckbox->setTooltipText(descriptiopn);
                pCheckbox->setPosition(xPos + textWidth, textField->getY());
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
    setHeight(y + pGameRuleManager->getVictoryRuleCount() * 50 + 50);
    pApp->continueThread();
}

void RuleSelection::startWeatherChanged(qint32 value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap::getInstance()->getGameRules()->setStartWeather(value);
    pApp->continueThread();
}

void RuleSelection::weatherChancesChanged()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    for (qint32 i = 0; i < GameMap::getInstance()->getGameRules()->getWeatherCount(); i++)
    {
        GameMap::getInstance()->getGameRules()->changeWeatherChance(i, m_pWeatherSlider->getSliderValue(i));
    }
    pApp->continueThread();
}

void RuleSelection::showCOBannlist()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    spCOBannListDialog pBannlist = new COBannListDialog(pMap->getGameRules()->getCOBannlist());
    oxygine::getStage()->addChild(pBannlist);
    connect(pBannlist.get(), &COBannListDialog::editFinished, pMap->getGameRules(), &GameRules::setCOBannlist, Qt::QueuedConnection);
    pApp->continueThread();
}
