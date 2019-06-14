#include "mapselectionmapsmenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/cospritemanager.h"

#include "resource_management/gamerulemanager.h"

#include "game/gamemap.h"

#include "game/campaign.h"

#include "game/player.h"

#include "game/co.h"

#include "menue/mainwindow.h"
#include "menue/gamemenue.h"
#include "menue/campaignmenu.h"
#include "multiplayer/multiplayermenu.h"

#include "objects/checkbox.h"
#include "objects/spinbox.h"

MapSelectionMapsMenue::MapSelectionMapsMenue(qint32 heigth, spMapSelectionView pMapSelectionView)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    Console::print("Entering Map Selection Menue", Console::eDEBUG);
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    pBuildingSpriteManager->loadAll();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(pApp->getSettings()->getWidth() / pBackground->getWidth());
    sprite->setScaleY(pApp->getSettings()->getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/mapselection");
    pApp->getAudioThread()->playRandom();

    if (pMapSelectionView.get() == nullptr)
    {
        m_pMapSelectionView = new MapSelectionView();
    }
    else
    {
        m_pMapSelectionView = pMapSelectionView;
    }
    addChild(m_pMapSelectionView);

    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemChanged, this, &MapSelectionMapsMenue::mapSelectionItemChanged, Qt::QueuedConnection);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemClicked, this, &MapSelectionMapsMenue::mapSelectionItemClicked, Qt::QueuedConnection);

    m_pButtonBack = ObjectManager::createButton(tr("Back"));
    m_pButtonBack->setPosition(10, pApp->getSettings()->getHeight() - 10 - m_pButtonBack->getHeight());
    m_pButtonBack->attachTo(this);
    m_pButtonBack->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonBack();
    });
    connect(this, &MapSelectionMapsMenue::buttonBack, this, &MapSelectionMapsMenue::slotButtonBack, Qt::QueuedConnection);

    m_pButtonNext = ObjectManager::createButton(tr("Next"));
    m_pButtonNext->setPosition(pApp->getSettings()->getWidth() - 10 - m_pButtonNext->getWidth(), pApp->getSettings()->getHeight() - 10 - m_pButtonNext->getHeight());
    m_pButtonNext->attachTo(this);
    m_pButtonNext->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonNext();
    });
    connect(this, &MapSelectionMapsMenue::buttonNext, this, &MapSelectionMapsMenue::slotButtonNext, Qt::QueuedConnection);

    m_pButtonStart = ObjectManager::createButton(tr("Start Game"));
    m_pButtonStart->setPosition(pApp->getSettings()->getWidth() - 10 - m_pButtonStart->getWidth(), pApp->getSettings()->getHeight() - 10 - m_pButtonStart->getHeight());
    m_pButtonStart->attachTo(this);
    m_pButtonStart->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonStartGame();
    });
    connect(this, &MapSelectionMapsMenue::buttonStartGame, this, &MapSelectionMapsMenue::startGame, Qt::QueuedConnection);

    qint32 yPos = 10;
    if (heigth <  0)
    {
        heigth = pApp->getSettings()->getHeight() - 40 * 2;
    }
    m_pPlayerSelection = new PlayerSelection(pApp->getSettings()->getWidth() - 20,
                                             heigth);
    m_pPlayerSelection->setPosition(10, yPos);
    addChild(m_pPlayerSelection);

    QSize size(pApp->getSettings()->getWidth() - 20, pApp->getSettings()->getHeight() - 40 * 2);
    m_pRuleSelection = new  Panel(true,  size, size);
    m_pRuleSelection->setPosition(10, 20);
    addChild(m_pRuleSelection);
    if (m_pMapSelectionView->getCurrentCampaign().get() == nullptr)
    {
        hidePlayerSelection();
        hideRuleSelection();
    }
    else
    {
        hideMapSelection();
        hideRuleSelection();
        m_pPlayerSelection->attachCampaign(m_pMapSelectionView->getCurrentCampaign());
        showPlayerSelection();
        m_MapSelectionStep = MapSelectionStep::selectMap;
    }
}

MapSelectionMapsMenue::~MapSelectionMapsMenue()
{

}

void MapSelectionMapsMenue::slotButtonBack()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            Console::print("Leaving Map Selection Menue", Console::eDEBUG);
            oxygine::getStage()->addChild(new Mainwindow());
            oxygine::Actor::detach();
            break;
        }
        case MapSelectionStep::selectRules:
        {
            hideRuleSelection();
            showMapSelection();
            m_MapSelectionStep = MapSelectionStep::selectMap;
            break;
        }
        case MapSelectionStep::selectPlayer:
        {
            if (m_pMapSelectionView->getCurrentCampaign().get() == nullptr)
            {
                showRuleSelection();
                hidePlayerSelection();
                m_MapSelectionStep = MapSelectionStep::selectRules;
            }
            else
            {
                m_pMapSelectionView->getMapSelection()->changeFolder("");
                hideRuleSelection();
                showMapSelection();
                m_MapSelectionStep = MapSelectionStep::selectMap;
            }
            break;
        }
    }
    pApp->continueThread();
}

void MapSelectionMapsMenue::slotButtonNext()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            QString file = m_pMapSelectionView->getMapSelection()->getCurrentFile();
            if (m_pMapSelectionView->getCurrentMap() != nullptr && file.endsWith(".map"))
            {
                m_pMapSelectionView->setCurrentCampaign(nullptr);
                if (m_pMapSelectionView->getCurrentMap()->getGameScript()->immediateStart())
                {
                    startGame();
                }
                else
                {
                    hideMapSelection();
                    showRuleSelection();
                    m_MapSelectionStep = MapSelectionStep::selectRules;
                }
            }
            else
            {
                if (file.endsWith(".jsm"))
                {
                    Console::print("Leaving Map Selection Menue", Console::eDEBUG);
                    if (dynamic_cast<Multiplayermenu*>(this) == nullptr)
                    {
                        oxygine::getStage()->addChild(new CampaignMenu(m_pMapSelectionView->getCurrentCampaign(), true));
                    }
                    else
                    {
                        oxygine::getStage()->addChild(new CampaignMenu(m_pMapSelectionView->getCurrentCampaign(), false));
                    }
                    oxygine::Actor::detach();
                }
            }
            break;
        }
        case MapSelectionStep::selectRules:
        {
            hideRuleSelection();
            showPlayerSelection();
            m_MapSelectionStep = MapSelectionStep::selectPlayer;
            break;
        }
        case MapSelectionStep::selectPlayer:
        {
            break;
        }
    }
    pApp->continueThread();
}

void MapSelectionMapsMenue::mapSelectionItemClicked(QString item)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QFileInfo info = m_pMapSelectionView->getMapSelection()->getCurrentFolder() + item;
    if (info.isFile())
    {
        emit buttonNext();
    }
    pApp->continueThread();
}

void MapSelectionMapsMenue::mapSelectionItemChanged(QString item)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QFileInfo info = m_pMapSelectionView->getMapSelection()->getCurrentFolder() + item;
    m_pMapSelectionView->loadMap(info);
    pApp->continueThread();
}



void MapSelectionMapsMenue::startWeatherChanged(qint32 value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pMapSelectionView->getCurrentMap()->getGameRules()->setStartWeather(value);
    pApp->continueThread();
}

void MapSelectionMapsMenue::weatherChancesChanged()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getGameRules()->getWeatherCount(); i++)
    {
        m_pMapSelectionView->getCurrentMap()->getGameRules()->changeWeatherChance(i, m_pWeatherSlider->getSliderValue(i));
    }
    pApp->continueThread();
}

void MapSelectionMapsMenue::hideMapSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pMapSelectionView->setVisible(false);
    pApp->continueThread();
}

void MapSelectionMapsMenue::showMapSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pMapSelectionView->setVisible(true);
    pApp->continueThread();
}

void MapSelectionMapsMenue::hideRuleSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pRuleSelection->setVisible(false);
    pApp->continueThread();
}

void MapSelectionMapsMenue::showRuleSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pRuleSelection->setVisible(true);
    m_pRuleSelection->clearContent();
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    qint32 y = 20;
    // font style
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    oxygine::spTextField textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Enviroment").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    y += 40;

    QVector<QString> weatherStrings;
    QVector<qint32> weatherChances;
    for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getGameRules()->getWeatherCount(); i++)
    {
        Weather* pWeather = m_pMapSelectionView->getCurrentMap()->getGameRules()->getWeather(i);
        weatherStrings.append(pWeather->getWeatherName());
        weatherChances.append(m_pMapSelectionView->getCurrentMap()->getGameRules()->getWeatherChance(pWeather->getWeatherId()));
    }
    m_pWeatherSlider = new Multislider(weatherStrings, pApp->getSettings()->getWidth() - 80, weatherChances);
    m_pWeatherSlider->setPosition(30, y);
    m_pRuleSelection->addItem(m_pWeatherSlider);
    connect(m_pWeatherSlider.get(), &Multislider::signalSliderChanged, this, &MapSelectionMapsMenue::weatherChancesChanged, Qt::QueuedConnection);

    y += m_pWeatherSlider->getHeight();
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Random Weather: ").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    spCheckbox pCheckbox = new Checkbox();
    pCheckbox->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
    m_pRuleSelection->addItem(pCheckbox);
    pCheckbox->setChecked(m_pMapSelectionView->getCurrentMap()->getGameRules()->getRandomWeather());
    connect(pCheckbox.get(), &Checkbox::checkChanged, m_pMapSelectionView->getCurrentMap()->getGameRules(), &GameRules::setRandomWeather, Qt::QueuedConnection);

    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Start Weather: ").toStdString().c_str());
    textField->setPosition(30, pCheckbox->getY() + 40);
    m_pRuleSelection->addItem(textField);

    spDropDownmenu startWeather = new DropDownmenu(200, weatherStrings);
    startWeather->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
    startWeather->setCurrentItem(m_pMapSelectionView->getCurrentMap()->getGameRules()->getCurrentWeatherId());
    connect(startWeather.get(), &DropDownmenu::sigItemChanged, this, &MapSelectionMapsMenue::startWeatherChanged, Qt::QueuedConnection);
    m_pRuleSelection->addItem(startWeather);
    startWeatherChanged(0);

    y = textField->getY() + 50;

    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Gameplay").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Unit Ranking System: ").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
    m_pRuleSelection->addItem(pCheckbox);
    pCheckbox->setChecked(m_pMapSelectionView->getCurrentMap()->getGameRules()->getRankingSystem());
    connect(pCheckbox.get(), &Checkbox::checkChanged, m_pMapSelectionView->getCurrentMap()->getGameRules(), &GameRules::setRankingSystem, Qt::QueuedConnection);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("No CO Powers: ").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    pCheckbox = new Checkbox();
    pCheckbox->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
    m_pRuleSelection->addItem(pCheckbox);
    pCheckbox->setChecked(m_pMapSelectionView->getCurrentMap()->getGameRules()->getNoPower());
    connect(pCheckbox.get(), &Checkbox::checkChanged, m_pMapSelectionView->getCurrentMap()->getGameRules(), &GameRules::setNoPower, Qt::QueuedConnection);
    y += 40;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Fog Of War: ").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    QVector<QString> fogModes = {tr("Off"), tr("Fog of War")};
    spDropDownmenu fogOfWar = new DropDownmenu(200, fogModes);
    fogOfWar->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
    fogOfWar->setCurrentItem(m_pMapSelectionView->getCurrentMap()->getGameRules()->getFogMode());
    connect(fogOfWar.get(), &DropDownmenu::sigItemChanged, m_pMapSelectionView->getCurrentMap()->getGameRules(), [=](qint32 value)
    {
        m_pMapSelectionView->getCurrentMap()->getGameRules()->setFogMode(static_cast<GameEnums::Fog>(value));
    });
    m_pRuleSelection->addItem(fogOfWar);
    y += 50;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Unit Limit: ").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    spSpinBox pSpinbox = new SpinBox(150, 0, 9999);
    pSpinbox->setInfinityValue(0.0f);
    pSpinbox->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
    m_pRuleSelection->addItem(pSpinbox);
    pSpinbox->setCurrentValue(m_pMapSelectionView->getCurrentMap()->getGameRules()->getUnitLimit());
    connect(pSpinbox.get(), &SpinBox::sigValueChanged, m_pMapSelectionView->getCurrentMap()->getGameRules(), &GameRules::setUnitLimit, Qt::QueuedConnection);

    y += 50;
    textField = new oxygine::TextField();
    textField->setStyle(style);
    textField->setText(tr("Victory Rules").toStdString().c_str());
    textField->setPosition(30, y);
    m_pRuleSelection->addItem(textField);
    y += 40;
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
                m_pMapSelectionView->getCurrentMap()->getGameRules()->addVictoryRule(pRule);
            }
            // add a cool check box and a cool text
            QString labelName = pRule->getRuleName();
            textField = new oxygine::TextField();
            textField->setStyle(style);
            textField->setText(labelName.toStdString().c_str());
            textField->setPosition(30, i * 50 + y);
            m_pRuleSelection->addItem(textField);
            spCheckbox pCheckbox = new Checkbox();
            pCheckbox->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
            m_pRuleSelection->addItem(pCheckbox);
            pCheckbox->setChecked(defaultValue);
            connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
            {
                if (value)
                {
                    m_pMapSelectionView->getCurrentMap()->getGameRules()->addVictoryRule(ruleID);
                }
                else
                {
                    m_pMapSelectionView->getCurrentMap()->getGameRules()->removeVictoryRule(ruleID);
                }
            });
        }
        else if (inputType == "spinbox")
        {
            qint32 defaultValue = pRule->getDefaultValue();
            qint32 startValue = pRule->getInfiniteValue();
            if (defaultValue != startValue)
            {
                m_pMapSelectionView->getCurrentMap()->getGameRules()->addVictoryRule(pRule);
            }
            QString labelName = pRule->getRuleName();
            textField = new oxygine::TextField();
            textField->setStyle(style);
            textField->setText(labelName.toStdString().c_str());
            textField->setPosition(30, i * 50 + y);
            m_pRuleSelection->addItem(textField);
            spSpinBox pSpinbox = new SpinBox(200, startValue, 9999);
            pSpinbox->setPosition(40 + textField->getTextRect().getWidth(), textField->getY());
            pSpinbox->setInfinityValue(startValue);
            m_pRuleSelection->addItem(pSpinbox);
            pSpinbox->setCurrentValue(defaultValue);
            connect(pSpinbox.get(), &SpinBox::sigValueChanged, [=](float value)
            {
                qint32 newValue = static_cast<qint32>(value);
                m_pMapSelectionView->getCurrentMap()->getGameRules()->removeVictoryRule(ruleID);
                if (newValue != startValue)
                {
                    spVictoryRule pRule = new VictoryRule(ruleID);
                    pRule->setRuleValue(newValue);
                    m_pMapSelectionView->getCurrentMap()->getGameRules()->addVictoryRule(pRule);
                }
            });
        }
    }
    m_pRuleSelection->setContentHeigth(90 + startWeather->getY() + pGameRuleManager->getVictoryRuleCount() * 50);
    pApp->continueThread();
}

void MapSelectionMapsMenue::showPlayerSelection()
{
    m_pButtonStart->setVisible(true);
    m_pButtonNext->setVisible(false);
    m_pPlayerSelection->setVisible(true);
    m_pPlayerSelection->showPlayerSelection();
}

void MapSelectionMapsMenue::hidePlayerSelection()
{
    m_pButtonStart->setVisible(false);
    m_pButtonNext->setVisible(true);
    m_pPlayerSelection->setVisible(false);
}

void MapSelectionMapsMenue::startGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->initPlayers();
    pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
    pMap->getGameScript()->gameStart();
    pMap->updateSprites();
    // start game
    Console::print("Leaving Map Selection Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new GameMenue());
    oxygine::Actor::detach();
    pApp->continueThread();
}
