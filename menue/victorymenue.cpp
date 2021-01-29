#include <QtMath>

#include "menue/victorymenue.h"
#include "menue/mainwindow.h"
#include "menue/campaignmenu.h"

#include "game/gamemap.h"
#include "game/gamerecording/gamerecorder.h"
#include "game/player.h"
#include "game/co.h"

#include "resource_management/gamemanager.h"
#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"
#include "coreengine/userdata.h"

VictoryMenue::VictoryMenue(spNetworkInterface pNetworkInterface)
    : QObject(),
      m_pNetworkInterface(pNetworkInterface)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Victory Menue", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headerStyle = FontManager::getMainFont72();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/victorymenue");
    pApp->getAudioThread()->playRandom();

    GameManager* pGameManager = GameManager::getInstance();
    m_pGraphBackground = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("graph");
    m_pGraphBackground->setResAnim(pAnim);
    qint32 widthCount = (Settings::getWidth() - 310) / static_cast<qint32>(pAnim->getWidth());
    qint32 heigthCount = (Settings::getHeight() - 40 - 105 - 100 - 20) / static_cast<qint32>(pAnim->getHeight());
    m_pGraphBackground->setSize(widthCount * pAnim->getWidth(), heigthCount * pAnim->getHeight());
    m_pGraphBackground->setVerticalMode(oxygine::Box9Sprite::TILING);
    m_pGraphBackground->setHorizontalMode(oxygine::Box9Sprite::TILING);
    m_pGraphBackground->setColor(255, 255, 255, 200);
    m_pGraphBackground->setGuides(0, pAnim->getWidth(), 0, pAnim->getHeight());
    m_pGraphBackground->setPosition(Settings::getWidth() - 10 - m_pGraphBackground->getWidth(), 110);
    addChild(m_pGraphBackground);
    qint32 graphDays = m_pGraphBackground->getWidth() / 100;
    for (qint32 i = 0; i < graphDays; i++)
    {
        oxygine::spTextField pDayText = new oxygine::TextField();
        pDayText->setY(m_pGraphBackground->getHeight());
        pDayText->setX(i * 100);
        pDayText->setStyle(style);
        qint32 day = static_cast<qint32>((i * pMap->getCurrentDay()) / graphDays) + 1;
        pDayText->setHtmlText(QString::number(day));
        m_pGraphBackground->addChild(pDayText);
    }
    qint32 graphHeigth = m_pGraphBackground->getHeight() / 100;
    for (qint32 i = 0; i < graphHeigth; i++)
    {
        m_YGraphItems.append(new oxygine::TextField());
        m_YGraphItems[i]->setY(i * 100);
        m_YGraphItems[i]->setX(-100);
        m_YGraphItems[i]->setStyle(style);
        m_pGraphBackground->addChild(m_YGraphItems[i]);
    }

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(Settings::getWidth() - pButtonExit->getWidth() - 10,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &VictoryMenue::sigExitMenue, this, &VictoryMenue::exitMenue, Qt::QueuedConnection);
    connect(this, &VictoryMenue::sigShowGraph, this, &VictoryMenue::showGraph, Qt::QueuedConnection);
    // initialize graph data

    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        m_VisiblePlayers.append(true);
    }
    for (qint32 i = 0; i < m_PlayerGraphs.size(); i++)
    {
        for (qint32 i2 = 0; i2 < pMap->getPlayerCount(); i2++)
        {
            m_PlayerGraphs[i].append(new oxygine::Actor());
            m_pGraphBackground->addChild(m_PlayerGraphs[i][i2]);
        }

        oxygine::spColorRectSprite pProgressLine = new oxygine::ColorRectSprite();
        pProgressLine->setColor(255, 0, 0, 255);
        pProgressLine->setSize(5, m_pGraphBackground->getHeight());
        pProgressLine->setPriority(10);
        m_GraphsProgessLine[i] = pProgressLine;
        m_pGraphBackground->addChild(pProgressLine);
    }
    // find max values for each graph here
    for (qint32 i = 0; i < pMap->getCurrentDay(); i++)
    {
        DayToDayRecord* pDayRecord = pMap->getGameRecorder()->getDayRecord(i);
        if (pDayRecord != nullptr)
        {
            for (qint32 i2 = 0; i2 < pMap->getPlayerCount(); i2++)
            {
                qint32 funds = pDayRecord->getPlayerRecord(i2)->getFunds();
                qint32 income = pDayRecord->getPlayerRecord(i2)->getIncome();
                qint32 buildings = pDayRecord->getPlayerRecord(i2)->getBuildings();
                qint32 units = pDayRecord->getPlayerRecord(i2)->getUnits();
                qint32 playerStrength = pDayRecord->getPlayerRecord(i2)->getPlayerStrength();
                if (funds > m_GraphMaxValues[static_cast<qint32>(GraphModes::Funds)])
                {
                    m_GraphMaxValues[static_cast<qint32>(GraphModes::Funds)] = funds;
                }
                if (income > m_GraphMaxValues[static_cast<qint32>(GraphModes::Income)])
                {
                    m_GraphMaxValues[static_cast<qint32>(GraphModes::Income)] = income;
                }
                if (buildings > m_GraphMaxValues[static_cast<qint32>(GraphModes::Buildings)])
                {
                    m_GraphMaxValues[static_cast<qint32>(GraphModes::Buildings)] = buildings;
                }
                if (units > m_GraphMaxValues[static_cast<qint32>(GraphModes::Units)])
                {
                    m_GraphMaxValues[static_cast<qint32>(GraphModes::Units)] = units;
                }
                if (playerStrength > m_GraphMaxValues[static_cast<qint32>(GraphModes::PlayerStrength)])
                {
                    m_GraphMaxValues[static_cast<qint32>(GraphModes::PlayerStrength)] = playerStrength;
                }
            }
        }
    }

    if (pMap->getCurrentDay() > 1)
    {
        lineLength = m_pGraphBackground->getWidth() / static_cast<float>(pMap->getCurrentDay() - 1);
    }
    else
    {
        lineLength = m_pGraphBackground->getWidth();
    }

    m_ProgressTimer.setSingleShot(false);

    m_ProgressTimer.start(getStepTime());
    connect(&m_ProgressTimer, &QTimer::timeout, this, &VictoryMenue::updateGraph, Qt::QueuedConnection);

    spPanel panel = new Panel(true, QSize(Settings::getWidth() - pButtonExit->getWidth() - 30, 105), QSize(Settings::getWidth() - pButtonExit->getX() - 20, 40));
    panel->setPosition(10, Settings::getHeight() - 105);
    addChild(panel);

    oxygine::spButton pButtonFunds = ObjectManager::createButton(tr("Funds"));
    pButtonFunds->attachTo(this);
    pButtonFunds->setPosition(5, 5);
    pButtonFunds->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Funds);
    });
    panel->addItem(pButtonFunds);

    oxygine::spButton pButtonIncome = ObjectManager::createButton(tr("Income"));
    pButtonIncome->attachTo(this);
    pButtonIncome->setPosition(10 + pButtonFunds->getWidth() + pButtonFunds->getX(), 5);
    pButtonIncome->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Income);
    });
    panel->addItem(pButtonIncome);

    oxygine::spButton pButtonBuildings = ObjectManager::createButton(tr("Buildings"));
    pButtonBuildings->attachTo(this);
    pButtonBuildings->setPosition(10 + pButtonIncome->getWidth() + pButtonIncome->getX(), 5);
    pButtonBuildings->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Buildings);
    });
    panel->addItem(pButtonBuildings);

    oxygine::spButton pButtonUnits = ObjectManager::createButton(tr("Units"));
    pButtonUnits->attachTo(this);
    pButtonUnits->setPosition(10 + pButtonBuildings->getWidth() + pButtonBuildings->getX(), 5);
    pButtonUnits->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Units);
    });
    panel->addItem(pButtonUnits);

    oxygine::spButton pButtonPlayerStrength = ObjectManager::createButton(tr("Player Strength"));
    pButtonPlayerStrength->attachTo(this);
    pButtonPlayerStrength->setPosition(10 + pButtonUnits->getWidth() + pButtonUnits->getX(), 5);
    pButtonPlayerStrength->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::PlayerStrength);
    });
    panel->addItem(pButtonPlayerStrength);

    if (pMap->getWinnerTeam() >= 0)
    {
        oxygine::spButton pButtonVictoryRanking = ObjectManager::createButton(tr("Ranking"));
        pButtonVictoryRanking->attachTo(this);
        pButtonVictoryRanking->setPosition(10 + pButtonPlayerStrength->getWidth() + pButtonPlayerStrength->getX(), 5);
        pButtonVictoryRanking->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit sigShowGraph(GraphModes::VictoryRanking);
        });
        panel->addItem(pButtonVictoryRanking);

        panel->setContentWidth(pButtonVictoryRanking->getX() + pButtonVictoryRanking->getWidth() + 5);
    }
    else
    {
        panel->setContentWidth(pButtonPlayerStrength->getX() + pButtonPlayerStrength->getWidth() + 5);
    }

    m_Textfield = new oxygine::TextField();
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    m_Textfield->setStyle(headerStyle);
    addChild(m_Textfield);

    m_PlayerSelectPanel = new Panel(true, QSize(200, m_pGraphBackground->getHeight()), QSize(200, 100));
    m_PlayerSelectPanel->setPosition(5, m_pGraphBackground->getY());
    addChild(m_PlayerSelectPanel);
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        oxygine::spTextField pTextfield = new oxygine::TextField();
        pTextfield->setStyle(style);
        pTextfield->setX(5);
        pTextfield->setY(50 * i + 5);
        pTextfield->setHtmlText((tr("Player: ") + QString::number(i + 1)));
        spCheckbox pCheckbox = new Checkbox();
        pCheckbox->setChecked(true);
        pCheckbox->setPosition(15 + pTextfield->getTextRect().getWidth(), pTextfield->getY());
        connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
        {
            m_VisiblePlayers[i] = value;
            emit sigShowGraph(m_CurrentGraphMode);
        });
        m_PlayerSelectPanel->addItem(pCheckbox);
        m_PlayerSelectPanel->addItem(pTextfield);
    }

    // victory score
    qint32 winnerTeam = pMap->getWinnerTeam();
    if (winnerTeam >= 0)
    {
        oxygine::TextStyle style48 = FontManager::getMainFont48();
        style48.color = FontManager::getFontColor();
        style48.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style48.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style48.multiline = true;
        m_VictoryPanel = new Panel(true, QSize(Settings::getWidth() - 10, Settings::getHeight() - 115),
                                   QSize(Settings::getWidth() - 10, Settings::getHeight() - 115));
        m_VictoryPanel->setPosition(5, 5);
        addChild(m_VictoryPanel);
        qint32 startX = Settings::getWidth() - 700;
        if (startX < 900)
        {
            startX = 900;
        }
        oxygine::spTextField pHeaders = new oxygine::TextField();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Speed"));
        pHeaders->setPosition(startX, 5);
        m_VictoryPanel->addItem(pHeaders);
        pHeaders = new oxygine::TextField();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Force"));
        pHeaders->setPosition(startX + 160 * 1, 5);
        m_VictoryPanel->addItem(pHeaders);
        pHeaders = new oxygine::TextField();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Tech."));
        pHeaders->setPosition(startX + 160 * 2, 5);
        m_VictoryPanel->addItem(pHeaders);
        pHeaders = new oxygine::TextField();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Total"));
        pHeaders->setPosition(startX + 160 * 3, 5);
        m_VictoryPanel->addItem(pHeaders);
        m_VictoryPanel->setContentWidth(startX + 200 * 4 + 10);
        qint32 y = 50;
        float scale = 2.0f;
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            QVector3D score;
            pMap->getGameRecorder()->calculateRang(i, score);
            if (score.x() < 0)
            {
                score.setX(0);
            }
            if (score.y() < 0)
            {
                score.setY(0);
            }
            if (score.z() < 0)
            {
                score.setZ(0);
            }
            m_VictoryScores.append(score);

            Player* pPlayer = pMap->getPlayer(i);
            for (quint8 i2 = 0; i2 < 2; i2++)
            {
                CO* pCO = pPlayer->getCO(i2);
                if (pCO != nullptr)
                {
                    qint32 sentenceWidth = startX - 10;
                    pAnim = pGameManager->getResAnim("dialogfield+mask");
                    oxygine::spSprite  pTextMask = new oxygine::Sprite();
                    pTextMask->setWidth(sentenceWidth);
                    pTextMask->setScaleX(pTextMask->getWidth() / pAnim->getWidth());
                    pTextMask->setResAnim(pAnim);
                    pTextMask->setPosition(5, 5 + y);
                    QColor color = pPlayer->getColor();
                    pTextMask->setColor(color);
                    m_VictoryPanel->addItem(pTextMask);

                    oxygine::spSprite pWinLooseSprite = new oxygine::Sprite();
                    pAnim = pGameManager->getResAnim("dialogfield");
                    pWinLooseSprite->setWidth(sentenceWidth);
                    pWinLooseSprite->setScaleX(pWinLooseSprite->getWidth() / pAnim->getWidth());
                    pWinLooseSprite->setResAnim(pAnim);
                    pWinLooseSprite->setPosition(5, 5 + y);
                    m_VictoryPanel->addItem(pWinLooseSprite);

                    oxygine::spSprite pCOSprite = new oxygine::Sprite();
                    pCOSprite->setScale(scale);
                    pCOSprite->setPosition(5, 11 + y);
                    QString resAnim = pCO->getCoID().toLower() + "+face";
                    pAnim = pCO->getResAnim(resAnim);
                    QString sentence = "";
                    if (pAnim != nullptr)
                    {
                        if (winnerTeam == pPlayer->getTeam())
                        {
                            sentence = pCO->getVictorySentence();
                            pCOSprite->setResAnim(pAnim, static_cast<qint32>(GameEnums::COMood_Happy));
                        }
                        else
                        {
                            sentence = pCO->getDefeatSentence();
                            pCOSprite->setResAnim(pAnim, static_cast<qint32>(GameEnums::COMood_Sad));
                        }
                    }
                    m_VictoryPanel->addItem(pCOSprite);

                    oxygine::spTextField winLooseText = new oxygine::TextField();
                    style.multiline = true;
                    winLooseText->setStyle(style48);
                    winLooseText->setSize(sentenceWidth - 110, pWinLooseSprite->getScaledHeight());
                    winLooseText->setPosition(48 * scale + 10, 1 + y);
                    winLooseText->setHtmlText(sentence);
                    m_VictoryPanel->addItem(winLooseText);
                    y += 48 * scale + 10;
                }

                if (i2 == 0)
                {
                    if (pCO == nullptr)
                    {
                        y += 48 * scale + 10;
                    }
                    style.multiline = false;
                    m_VictoryTexts.append(QVector<oxygine::spTextField>());
                    m_VictoryTexts[i].append(new oxygine::TextField);
                    m_VictoryTexts[i][0]->setPosition(startX, y - 48 * scale);
                    m_VictoryTexts[i][0]->setStyle(style48);
                    m_VictoryTexts[i][0]->setHtmlText("0");
                    m_VictoryPanel->addItem(m_VictoryTexts[i][0]);
                    m_VictoryTexts[i].append(new oxygine::TextField);
                    m_VictoryTexts[i][1]->setPosition(startX + 160 * 1, y - 48 * scale);
                    m_VictoryTexts[i][1]->setStyle(style48);
                    m_VictoryTexts[i][1]->setHtmlText("0");
                    m_VictoryPanel->addItem(m_VictoryTexts[i][1]);
                    m_VictoryTexts[i].append(new oxygine::TextField);
                    m_VictoryTexts[i][2]->setPosition(startX + 160 * 2, y - 48 * scale);
                    m_VictoryTexts[i][2]->setStyle(style48);
                    m_VictoryTexts[i][2]->setHtmlText("0");
                    m_VictoryPanel->addItem(m_VictoryTexts[i][2]);
                    m_VictoryTexts[i].append(new oxygine::TextField);
                    m_VictoryTexts[i][3]->setPosition(startX + 160 * 3, y - 48 * scale);
                    m_VictoryTexts[i][3]->setStyle(style48);
                    m_VictoryTexts[i][3]->setHtmlText("0");
                    m_VictoryPanel->addItem(m_VictoryTexts[i][3]);
                }
            }
        }
        m_VictoryPanel->setContentHeigth(y + 48 * scale + 10);
        addShopMoney();
    }
    showGraph(GraphModes::Funds);

    m_pGraphBackground->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            emit sigFinishCurrentGraph();
            pTouchEvent->stopPropagation();
        }
    });
    connect(this, &VictoryMenue::sigFinishCurrentGraph, this, &VictoryMenue::finishGraph, Qt::QueuedConnection);
    pApp->continueRendering();
    if (Mainapp::getSlave())
    {
        // despawn slave process on finish
        Console::print("Closing slave cause the game is finished.", Console::eDEBUG);
        QCoreApplication::exit(0);
    }
    else
    {
        AddScoreToUserdata();
    }
    connect(this, &VictoryMenue::sigOnEnter, this, &VictoryMenue::onEnter, Qt::QueuedConnection);
    emit sigOnEnter();
}

void VictoryMenue::addShopMoney()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 highestScore = 0;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (pMap->getPlayer(i)->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
        {
            qint32 score = m_VictoryScores[i].x() + m_VictoryScores[i].y() + m_VictoryScores[i].z();
            if (score > highestScore)
            {
                highestScore = score;
            }
        }
    }
    Userdata* pUserdata = Userdata::getInstance();
    pUserdata->addCredtis(highestScore);
}

void VictoryMenue::showGraph(VictoryMenue::GraphModes mode)
{
    
    m_CurrentGraphMode = mode;
    if (m_CurrentGraphMode < GraphModes::Max)
    {
        m_ProgressTimer.stop();
        m_ProgressTimer.start(getStepTime());
        m_PlayerSelectPanel->setVisible(true);
        m_pGraphBackground->setVisible(true);
        if (m_VictoryPanel.get() != nullptr)
        {
            m_VictoryPanel->setVisible(false);
        }

        for (qint32 i = 0; i < m_YGraphItems.size(); i++)
        {
            m_YGraphItems[i]->setHtmlText(QString::number(static_cast<qint32>(m_GraphMaxValues[static_cast<qint32>(m_CurrentGraphMode)]
                                                          * (1.0f - i / static_cast<float>(m_YGraphItems.size())))));
        }

        switch (m_CurrentGraphMode)
        {
            case GraphModes::Funds:
            {
                m_Textfield->setHtmlText(tr("Funds"));
                break;
            }
            case GraphModes::Income:
            {
                m_Textfield->setHtmlText(tr("Income"));
                break;
            }
            case GraphModes::Buildings:
            {
                m_Textfield->setHtmlText(tr("Buildings"));
                break;
            }
            case GraphModes::Units:
            {
                m_Textfield->setHtmlText(tr("Units"));
                break;
            }
            case GraphModes::PlayerStrength:
            {
                m_Textfield->setHtmlText(tr("Player Strength"));
                break;
            }
            case GraphModes::Max:
            case GraphModes::VictoryRanking:
            {
                break;
            }
        }


        for (qint32 i = 0; i < m_PlayerGraphs.size(); i++)
        {
            if (i == static_cast<qint32>(mode))
            {
                m_GraphsProgessLine[i]->setVisible(true);
            }
            else
            {
                m_GraphsProgessLine[i]->setVisible(false);
            }
            for (qint32 i2 = 0; i2 < m_PlayerGraphs[i].size(); i2++)
            {
                if (i == static_cast<qint32>(mode))
                {
                    if (m_VisiblePlayers[i2])
                    {
                        m_PlayerGraphs[i][i2]->setVisible(true);
                    }
                    else
                    {
                        m_PlayerGraphs[i][i2]->setVisible(false);
                    }
                }
                else
                {
                    m_PlayerGraphs[i][i2]->setVisible(false);
                }
            }
        }
    }
    else
    {
        m_ProgressTimer.stop();
        m_ProgressTimer.start(50);
        if (m_VictoryPanel.get() != nullptr)
        {
            m_PlayerSelectPanel->setVisible(false);
            m_pGraphBackground->setVisible(false);
            m_VictoryPanel->setVisible(true);
            m_Textfield->setHtmlText(tr("Victory"));
        }
    }
    m_Textfield->setX(Settings::getWidth() / 2.0f - m_Textfield->getTextRect().getWidth() / 2.0f);

    
}

void VictoryMenue::exitMenue()
{
    
    spCampaign campaign = GameMap::getInstance()->getSpCampaign();
    if (campaign.get() != nullptr && campaign->getCampaignFinished() == false)
    {
        GameMap::deleteMap();
        Console::print("Leaving Victory Menue", Console::eDEBUG);
        bool multiplayer = m_pNetworkInterface.get() != nullptr;
        oxygine::getStage()->addChild(new CampaignMenu(campaign, multiplayer));
        oxygine::Actor::detach();
    }
    else
    {
        GameMap::deleteMap();
        Console::print("Leaving Victory Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new Mainwindow());
        oxygine::Actor::detach();
    }

    if (m_pNetworkInterface.get() != nullptr)
    {
        emit m_pNetworkInterface->sig_close();
        m_pNetworkInterface = nullptr;
    }
    
}

oxygine::spActor VictoryMenue::createLine(QPointF end, qint32 lineWidth, QColor color)
{
    oxygine::spActor pRet = new oxygine::Actor();
    float m = static_cast<float>(end.y()) / static_cast<float>(end.x());
    qint32 width = lineWidth;
    if (end.y() != 0)
    {
        width = qSqrt(lineWidth * lineWidth * m * m / (m * m + 1));
    }
    oxygine::spColorRectSprite rect = new oxygine::ColorRectSprite();
    rect->setColor(color);
    rect->setPosition(0, 0);
    rect->setSize(lineWidth, lineWidth);
    pRet->addChild(rect);
    double angle = 0;

    rect = new oxygine::ColorRectSprite();
    rect->setColor(color);


    qint32 x = end.x() - width;
    if (x > 0)
    {
        angle = qAtan(end.y() / x);
    }

    qint32 y = end.y();
    if (angle >= 0.0)
    {
        rect->setPosition(width, 0);
    }
    else
    {
        rect->setPosition(0, 0);
    }
    qint32 lineLength = qSqrt(x * x + y * y);
    if (angle >= 1.0)
    {
        lineLength += lineWidth / 2;
    }
    rect->setSize(lineLength, lineWidth);
    rect->setRotation(angle);
    pRet->addChild(rect);
    rect = new oxygine::ColorRectSprite();
    rect->setColor(color);
    rect->setPosition(end.x() - lineWidth, end.y());
    rect->setSize(lineWidth, lineWidth);
    pRet->addChild(rect);
    return pRet;
}

void VictoryMenue::updateGraph()
{    
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        if (m_CurrentGraphMode < GraphModes::Max)
        {
            // make sure we have some graph data to be added :)
            qint32 progress = m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)];
            if (progress < pMap->getCurrentDay())
            {
                drawGraphStep(progress);
                m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)]++;
            }
        }
        else
        {
            if (progress < 100)
            {
                progress += 2;
                // update values
                for (qint32 i = 0; i < m_VictoryTexts.size(); i++)
                {
                    m_VictoryTexts[i][0]->setHtmlText(QString::number(static_cast<qint32>(m_VictoryScores[i].x() * progress / 100)));
                    m_VictoryTexts[i][1]->setHtmlText(QString::number(static_cast<qint32>(m_VictoryScores[i].y() * progress / 100)));
                    m_VictoryTexts[i][2]->setHtmlText(QString::number(static_cast<qint32>(m_VictoryScores[i].z() * progress / 100)));
                    float sum = m_VictoryScores[i].x() + m_VictoryScores[i].y() +m_VictoryScores[i].z();
                    m_VictoryTexts[i][3]->setHtmlText(QString::number(static_cast<qint32>(sum * progress / 100)));
                }
                if (progress >= 100)
                {
                    progress = 100;
                    // show CO-Rank
                    for (qint32 i = 0; i < m_VictoryTexts.size(); i++)
                    {
                        oxygine::ResAnim* pAnim = nullptr;
                        qint32 sum = static_cast<qint32>(m_VictoryScores[i].x() + m_VictoryScores[i].y() +m_VictoryScores[i].z());
                        GameRecorder::Rang rang = pMap->getGameRecorder()->getRank(sum);
                        pAnim = pMap->getGameRecorder()->getRankAnim(rang);
                        oxygine::spSprite pRankSprite = new oxygine::Sprite();
                        pRankSprite->setResAnim(pAnim);
                        pRankSprite->setScale(1.5f);
                        pRankSprite->setPosition(m_VictoryPanel->getContentWidth() - 120, m_VictoryTexts[i][0]->getY());
                        m_VictoryPanel->addItem(pRankSprite);
                    }
                }
            }
        }
    }
}

void VictoryMenue::finishGraph()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 progress = m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)];
    while (progress < pMap->getCurrentDay())
    {
        drawGraphStep(progress);
        m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)]++;
        progress = m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)];
    }
}

void VictoryMenue::drawGraphStep(qint32 progress)
{
    GameManager* pGameManager = GameManager::getInstance();
    spGameMap pMap = GameMap::getInstance();
    DayToDayRecord* pStartRecord = nullptr;
    DayToDayRecord* pEndRecord = nullptr;
    if (pMap->getCurrentDay() == 1)
    {
        pStartRecord = pMap->getGameRecorder()->getDayRecord(0);
        pEndRecord = pMap->getGameRecorder()->getDayRecord(0);
    }
    else if (progress < pMap->getCurrentDay() - 1)
    {
        pStartRecord = pMap->getGameRecorder()->getDayRecord(progress);
        pEndRecord = pMap->getGameRecorder()->getDayRecord(progress + 1);
    }
    if (pStartRecord != nullptr &&
        pEndRecord != nullptr)
    {
        m_GraphsProgessLine[static_cast<qint32>(m_CurrentGraphMode)]->setX((progress + 1) * lineLength - m_GraphsProgessLine[static_cast<qint32>(m_CurrentGraphMode)]->getWidth() / 2);
        // add player lines
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            QPointF startPoint(-1, -1);
            QPointF endPoint(-1, -1);

            startPoint.setX(progress * lineLength);
            endPoint.setX((progress + 1) * lineLength);

            switch (m_CurrentGraphMode)
            {
                case GraphModes::Funds:
                {
                    startPoint.setY(pStartRecord->getPlayerRecord(i)->getFunds());
                    endPoint.setY(pEndRecord->getPlayerRecord(i)->getFunds());
                    break;
                }
                case GraphModes::Income:
                {
                    startPoint.setY(pStartRecord->getPlayerRecord(i)->getIncome());
                    endPoint.setY(pEndRecord->getPlayerRecord(i)->getIncome());
                    break;
                }
                case GraphModes::Buildings:
                {
                    startPoint.setY(pStartRecord->getPlayerRecord(i)->getBuildings());
                    endPoint.setY(pEndRecord->getPlayerRecord(i)->getBuildings());
                    break;
                }
                case GraphModes::Units:
                {
                    startPoint.setY(pStartRecord->getPlayerRecord(i)->getUnits());
                    endPoint.setY(pEndRecord->getPlayerRecord(i)->getUnits());
                    break;
                }
                case GraphModes::PlayerStrength:
                {
                    startPoint.setY(pStartRecord->getPlayerRecord(i)->getPlayerStrength());
                    endPoint.setY(pEndRecord->getPlayerRecord(i)->getPlayerStrength());
                    break;
                }
                case GraphModes::Max:
                case GraphModes::VictoryRanking:
                {
                    break;
                }
            }
            if (startPoint.y() >= 0 && endPoint.y() < 0)
            {
                endPoint.setY(startPoint.y());
            }
            if (m_GraphMaxValues[static_cast<qint32>(m_CurrentGraphMode)] > 0)
            {
                startPoint.setY(startPoint.y() / m_GraphMaxValues[static_cast<qint32>(m_CurrentGraphMode)]);
                endPoint.setY(endPoint.y() / m_GraphMaxValues[static_cast<qint32>(m_CurrentGraphMode)]);
            }
            if (startPoint.y() >= 0 && endPoint.y() >= 0)
            {
                qint32 lineWidth = 5;
                startPoint.setY(m_pGraphBackground->getHeight() - startPoint.y() * m_pGraphBackground->getHeight());
                endPoint.setY(m_pGraphBackground->getHeight() - endPoint.y() * m_pGraphBackground->getHeight());
                oxygine::spActor line = createLine(endPoint - startPoint, lineWidth, pMap->getPlayer(i)->getColor());
                line->setPosition(startPoint.x(), startPoint.y() - lineWidth / 2);
                m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][i]->addChild(line);
            }
            for (qint32 event = 0; event < pStartRecord->getEventRecordCount(); event++)
            {
                SpecialEvent* pEvent = pStartRecord->getSpecialEvent(event);
                if (pEvent->getOwner() == i)
                {
                    oxygine::spSprite pSprite = new oxygine::Sprite();
                    oxygine::ResAnim* pAnim = nullptr;
                    switch (pEvent->getEvent())
                    {
                        case GameEnums::GameRecord_SpecialEvents_Power:
                        {
                            pAnim = pGameManager->getResAnim("power");
                            break;
                        }
                        case GameEnums::GameRecord_SpecialEvents_SuperPower:
                        {
                            pAnim = pGameManager->getResAnim("superpower");
                            break;
                        }
                        case GameEnums::GameRecord_SpecialEvents_HQCaptured:
                        {
                            pAnim = pGameManager->getResAnim("hq+captured");
                            QColor color = pMap->getPlayer(i)->getColor();
                            pSprite->setColor(color.red(), color.green(), color.blue(), color.alpha());
                            break;
                        }
                        case GameEnums::GameRecord_SpecialEvents_HQLost:
                        {
                            pAnim = pGameManager->getResAnim("hq+lost");
                            QColor color = pMap->getPlayer(i)->getColor();
                            pSprite->setColor(color.red(), color.green(), color.blue(), color.alpha());
                            break;
                        }
                    }
                    pSprite->setResAnim(pAnim);
                    pSprite->setScale(2.0f);
                    if (startPoint.y() < 0)
                    {
                        if (progress >= 0)
                        {
                            oxygine::spActor pActor = m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][i]->getLastChild();
                            if (pActor.get() != nullptr)
                            {
                                startPoint.setY(pActor->getY());
                            }
                        }
                    }
                    pSprite->setPosition(startPoint.x() - pAnim->getWidth(), startPoint.y() - pAnim->getHeight());
                    m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][i]->addChild(pSprite);
                }
            }
        }
    }

}

qint32 VictoryMenue::getStepTime()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 stepTime = static_cast<qint32>(15.0 * (20.0 - qExp(pMap->getCurrentDay() / 10.0)));
    if (pMap->getCurrentDay() < 5)
    {
        stepTime = 400;
    }
    else if (stepTime < 50)
    {
        stepTime = 50;
    }
    return stepTime;
}

void VictoryMenue::AddScoreToUserdata()
{
    spGameMap pMap = GameMap::getInstance();
    QString path = pMap->getMapPath();
    if (!path.isEmpty() && pMap->getWinnerTeam() >= 0)
    {
        qint32 playerCount = pMap->getPlayerCount();
        qint32 bestPlayer = -1;
        qint32 bestScore = -1;
        for (qint32 i = 0; i < playerCount; ++i)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes::AiTypes_Human &&
                pPlayer->getTeam() == pMap->getWinnerTeam())
            {
                qint32 score = m_VictoryScores[i].x() + m_VictoryScores[i].y() + m_VictoryScores[i].z();
                if (bestScore < score)
                {
                    bestScore = score;
                    bestPlayer = i;
                }
            }
        }
        if (bestPlayer >= 0)
        {
            Player* pPlayer = pMap->getPlayer(bestPlayer);
            QString co1;
            QString co2;
            if (pPlayer->getCO(0) != nullptr)
            {
                co1 = pPlayer->getCO(0)->getCoID();
            }
            if (pPlayer->getCO(1) != nullptr)
            {
                co2 = pPlayer->getCO(1)->getCoID();
            }
            Userdata::getInstance()->addVictoryForMap(path, co1, co2, bestScore);
        }
    }
}

void VictoryMenue::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "onVictory";
    if (pInterpreter->exists(object, func))
    {
        QJSValueList args;
        QJSValue value = pInterpreter->newQObject(this);
        args << value;
        pInterpreter->doFunction(object, func, args);
    }
}
