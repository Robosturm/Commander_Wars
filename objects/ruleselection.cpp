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

void RuleSelection::showRuleSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    removeChildren();
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    qint32 y = 20;
    // font style
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    oxygine::Color headerColor(0, 255, 0, 255);
    float headerScale = 1.5f;

    oxygine::spTextField textField = new oxygine::TextField();
    style.color = headerColor;
    textField->setStyle(style);
    style.color = oxygine::Color(255, 255, 255, 255);
    textField->setScale(headerScale);
    textField->setHtmlText(tr("Enviroment").toStdString().c_str());
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
    m_pWeatherSlider->setPosition(30, y);
    addChild(m_pWeatherSlider);
    connect(m_pWeatherSlider.get(), &Multislider::signalSliderChanged, this, &RuleSelection::weatherChancesChanged, Qt::QueuedConnection);

    y += m_pWeatherSlider->getHeight();
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Random Weather: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    spCheckbox pCheckbox = new Checkbox();
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getRandomWeather());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setRandomWeather, Qt::QueuedConnection);
    y += 40;

    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Start Weather: ").toStdString().c_str());
    textField->setPosition(30, pCheckbox->getY() + 40);
    addChild(textField);

    spDropDownmenu startWeather = new DropDownmenu(200, weatherStrings);
    startWeather->setPosition(textWidth, textField->getY());
    startWeather->setCurrentItem(pMap->getGameRules()->getCurrentWeatherId());
    connect(startWeather.get(), &DropDownmenu::sigItemChanged, this, &RuleSelection::startWeatherChanged, Qt::QueuedConnection);
    addChild(startWeather);
    startWeatherChanged(0);
    y += 50;

    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Weather Forecast: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getWeatherPrediction());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setWeatherPrediction, Qt::QueuedConnection);

    y = textField->getY() + 50;
    textField = new oxygine::TextField();
    style.color = headerColor;
    textField->setStyle(style);
    style.color = oxygine::Color(255, 255, 255, 255);
    textField->setScale(headerScale);
    textField->setHtmlText(tr("Gameplay").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    y += 40 * headerScale;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Unit Ranking System: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getRankingSystem());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setRankingSystem, Qt::QueuedConnection);

    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("CO Bannlist: ").toStdString().c_str());
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
    textField->setHtmlText(tr("No CO Powers: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getNoPower());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setNoPower, Qt::QueuedConnection);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("AI Attack Terrain: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setPosition(textWidth, textField->getY());
    addChild(pCheckbox);
    pCheckbox->setChecked(pMap->getGameRules()->getAiAttackTerrain());
    connect(pCheckbox.get(), &Checkbox::checkChanged, pMap->getGameRules(), &GameRules::setAiAttackTerrain, Qt::QueuedConnection);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Fog Of War: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    QVector<QString> fogModes = {tr("Off"), tr("Fog of War")};
    spDropDownmenu fogOfWar = new DropDownmenu(200, fogModes);
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
    textField->setHtmlText(tr("Unit Limit: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    spSpinBox pSpinbox = new SpinBox(150, 0, 9999);
    pSpinbox->setInfinityValue(0.0);
    pSpinbox->setPosition(textWidth, textField->getY());
    addChild(pSpinbox);
    pSpinbox->setCurrentValue(pMap->getGameRules()->getUnitLimit());
    connect(pSpinbox.get(), &SpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setUnitLimit, Qt::QueuedConnection);

    y += 50;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setHtmlText(tr("Round Time: ").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    spTimeSpinBox pTimeSpinbox = new TimeSpinBox(150);
    pTimeSpinbox->setPosition(textWidth, textField->getY());
    addChild(pTimeSpinbox);
    pTimeSpinbox->setCurrentValue(pMap->getGameRules()->getRoundTimeMs());
    connect(pTimeSpinbox.get(), &TimeSpinBox::sigValueChanged, pMap->getGameRules(), &GameRules::setRoundTimeMs, Qt::QueuedConnection);


    y += 50;
    textField = new oxygine::TextField();
    style.color = headerColor;
    textField->setStyle(style);
    style.color = oxygine::Color(255, 255, 255, 255);
    textField->setScale(headerScale);
    textField->setHtmlText(tr("Victory Rules").toStdString().c_str());
    textField->setPosition(30, y);
    addChild(textField);
    y += 40 * headerScale;
    for (qint32 i = 0; i < pGameRuleManager->getVictoryRuleCount(); i++)
    {
        QString ruleID = pGameRuleManager->getVictoryRuleID(i);
        spVictoryRule pRule = new VictoryRule(ruleID);
        QString inputType = pRule->getRuleType().toLower();
        if (inputType == "checkbox")
        {
            bool defaultValue = pRule->getDefaultValue();
            if (defaultValue)
            {
                pMap->getGameRules()->addVictoryRule(pRule);
            }
            // add a cool check box and a cool text
            QString labelName = pRule->getRuleName();
            textField = new oxygine::TextField();
            textField->setStyle(style);
            textField->setHtmlText(labelName.toStdString().c_str());
            textField->setPosition(30, i * 50 + y);
            addChild(textField);
            spCheckbox pCheckbox = new Checkbox();
            pCheckbox->setPosition(textWidth, textField->getY());
            addChild(pCheckbox);
            pCheckbox->setChecked(defaultValue);
            connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
            {
                if (value)
                {
                    pMap->getGameRules()->addVictoryRule(ruleID);
                }
                else
                {
                    pMap->getGameRules()->removeVictoryRule(ruleID);
                }
            });
        }
        else if (inputType == "spinbox")
        {
            qint32 defaultValue = pRule->getDefaultValue();
            qint32 startValue = pRule->getInfiniteValue();
            if (defaultValue != startValue)
            {
                pMap->getGameRules()->addVictoryRule(pRule);
            }
            QString labelName = pRule->getRuleName();
            textField = new oxygine::TextField();
            textField->setStyle(style);
            textField->setHtmlText(labelName.toStdString().c_str());
            textField->setPosition(30, i * 50 + y);
            addChild(textField);
            spSpinBox pSpinbox = new SpinBox(200, startValue, 9999);
            pSpinbox->setPosition(textWidth, textField->getY());
            pSpinbox->setInfinityValue(startValue);
            addChild(pSpinbox);
            pSpinbox->setCurrentValue(defaultValue);
            connect(pSpinbox.get(), &SpinBox::sigValueChanged, [=](float value)
            {
                qint32 newValue = static_cast<qint32>(value);
                pMap->getGameRules()->removeVictoryRule(ruleID);
                if (newValue != startValue)
                {
                    spVictoryRule pRule = new VictoryRule(ruleID);
                    pRule->setRuleValue(newValue);
                    pMap->getGameRules()->addVictoryRule(pRule);
                }
            });
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
