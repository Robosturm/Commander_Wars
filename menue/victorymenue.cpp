#include <QtMath>
#include <QJsonObject>
#include <QJsonArray>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "ai/aiprocesspipe.h"

#include "menue/victorymenue.h"
#include "menue/mainwindow.h"
#include "menue/campaignmenu.h"

#include "game/gamerecording/gamerecorder.h"
#include "game/player.h"
#include "game/co.h"

#include "resource_management/gamemanager.h"
#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"
#include "coreengine/audiomanager.h"
#include "coreengine/userdata.h"

#include "objects/dialogs/dialogvaluecounter.h"
#include "objects/base/moveinbutton.h"
#include "objects/base/checkbox.h"

#include "network/JsonKeys.h"
#include "network/tcpclient.h"

VictoryMenue::VictoryMenue(spGameMap pMap, spNetworkInterface pNetworkInterface, bool isReplay)
    : m_ProgressTimer(this),
      m_despawnSlaveTimer(this),
      m_pNetworkInterface(pNetworkInterface),
      m_pMap(pMap),
      m_isReplay(isReplay)
{
#ifdef GRAPHICSUPPORT
    setObjectName("VictoryMenue");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    CONSOLE_PRINT("Entering Victory Menue", GameConsole::eDEBUG);
    GlobalUtils::setUseSeed(false);
    m_pMap->setMenu(nullptr); // remove outdated link

    if (Mainapp::getSlave())
    {
        CONSOLE_PRINT("VictoryMenue is listening to master server data", GameConsole::eDEBUG);
        spTCPClient pSlaveMasterConnection = Mainapp::getSlaveClient();
        connect(pSlaveMasterConnection.get(), &TCPClient::recieveData, this, &VictoryMenue::recieveServerData, NetworkCommands::UNIQUE_DATA_CONNECTION);
    }
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getFont("mainBlack24"));
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getFont("mainBlack72"));
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("victorymenu");
    if (pBackground != nullptr)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pBackground->getWidth()));
        sprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pBackground->getHeight()));
    }

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/victorymenue");
    pApp->getAudioManager()->playRandom();

    GameManager* pGameManager = GameManager::getInstance();
    m_pGraphBackground = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("graph");
    if (pAnim != nullptr)
    {
        m_pGraphBackground->setResAnim(pAnim);
        qint32 widthCount = 0;
        if (Settings::getSmallScreenDevice())
        {
            widthCount = (oxygine::Stage::getStage()->getWidth() - 110) / static_cast<qint32>(pAnim->getWidth());
        }
        else
        {
            widthCount = (oxygine::Stage::getStage()->getWidth() - 310) / static_cast<qint32>(pAnim->getWidth());
        }
        qint32 heigthCount = 0;
        if (Settings::getSmallScreenDevice())
        {
            heigthCount = (oxygine::Stage::getStage()->getHeight() - 50) / static_cast<qint32>(pAnim->getHeight());
        }
        else
        {
            heigthCount = (oxygine::Stage::getStage()->getHeight() - 225) / static_cast<qint32>(pAnim->getHeight());
        }

        m_pGraphBackground->setSize(widthCount * pAnim->getWidth(), heigthCount * pAnim->getHeight());
        m_pGraphBackground->setVerticalMode(oxygine::Box9Sprite::TILING);
        m_pGraphBackground->setHorizontalMode(oxygine::Box9Sprite::TILING);
        m_pGraphBackground->setColor(255, 255, 255, 200);
        m_pGraphBackground->setGuides(0, pAnim->getWidth(), 0, pAnim->getHeight());
    }
    if (Settings::getSmallScreenDevice())
    {
        m_pGraphBackground->setPosition(oxygine::Stage::getStage()->getWidth() - 10 - m_pGraphBackground->getScaledWidth(), 10);
    }
    else
    {
        m_pGraphBackground->setPosition(oxygine::Stage::getStage()->getWidth() - 10 - m_pGraphBackground->getScaledWidth(), 90);
    }

    addChild(m_pGraphBackground);
    qint32 graphDays = m_pGraphBackground->getScaledWidth() / 100;
    for (qint32 i = 0; i < graphDays; i++)
    {
        oxygine::spTextField pDayText = oxygine::spTextField::create();
        pDayText->setY(m_pGraphBackground->getScaledHeight());
        pDayText->setX(i * 100);
        pDayText->setStyle(style);
        qint32 day = static_cast<qint32>((i * m_pMap->getCurrentDay()) / graphDays) + 1;
        pDayText->setHtmlText(QString::number(day));
        m_pGraphBackground->addChild(pDayText);
    }
    qint32 graphHeigth = m_pGraphBackground->getScaledHeight() / 100;
    for (qint32 i = 0; i < graphHeigth; i++)
    {
        m_YGraphItems.append(oxygine::spTextField::create());
        m_YGraphItems[i]->setY(i * 100);
        m_YGraphItems[i]->setX(-100);
        m_YGraphItems[i]->setStyle(style);
        m_pGraphBackground->addChild(m_YGraphItems[i]);
    }

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    addChild(pButtonExit);
    pButtonExit->setPosition(oxygine::Stage::getStage()->getWidth() - pButtonExit->getScaledWidth() - 10,
                             oxygine::Stage::getStage()->getHeight() - pButtonExit->getScaledHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &VictoryMenue::sigExitMenue, this, &VictoryMenue::exitMenue, Qt::QueuedConnection);
    connect(this, &VictoryMenue::sigShowGraph, this, &VictoryMenue::showGraph, Qt::QueuedConnection);
    // initialize graph data

    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_VisiblePlayers.append(true);
    }
    for (qint32 i = 0; i < m_PlayerGraphs.size(); i++)
    {
        for (qint32 i2 = 0; i2 < m_pMap->getPlayerCount(); i2++)
        {
            m_PlayerGraphs[i].append(oxygine::spActor::create());
            m_pGraphBackground->addChild(m_PlayerGraphs[i][i2]);
        }

        oxygine::spColorRectSprite pProgressLine = oxygine::spColorRectSprite::create();
        pProgressLine->setColor(255, 0, 0, 255);
        pProgressLine->setSize(5, m_pGraphBackground->getScaledHeight());
        pProgressLine->setPriority(10);
        m_GraphsProgessLine[i] = pProgressLine;
        m_pGraphBackground->addChild(pProgressLine);
    }
    // find max values for each graph here
    for (qint32 i = 0; i < m_pMap->getCurrentDay(); i++)
    {
        DayToDayRecord* pDayRecord = m_pMap->getGameRecorder()->getDayRecord(i);
        if (pDayRecord != nullptr)
        {
            for (qint32 i2 = 0; i2 < m_pMap->getPlayerCount(); i2++)
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

    if (m_pMap->getCurrentDay() > 1)
    {
        m_lineLength = m_pGraphBackground->getScaledWidth() / static_cast<float>(m_pMap->getCurrentDay() - 1);
    }
    else
    {
        m_lineLength = m_pGraphBackground->getScaledWidth();
    }

    m_ProgressTimer.setSingleShot(false);
    m_ProgressTimer.setInterval(getStepTime());
    connect(&m_ProgressTimer, &QTimer::timeout, this, &VictoryMenue::updateGraph, Qt::QueuedConnection);
    spPanel panel = spPanel::create(true, QSize(oxygine::Stage::getStage()->getWidth() - pButtonExit->getScaledWidth() - 30, 105), QSize(oxygine::Stage::getStage()->getWidth() - pButtonExit->getX() - 20, 40));

    if (Settings::getSmallScreenDevice())
    {
        panel->setPosition(10, oxygine::Stage::getStage()->getHeight() - 1);
        auto moveInButton = spMoveInButton::create(panel.get(), panel->getScaledHeight(), -1, -1, 2.0f, true);
    }
    else
    {
        panel->setPosition(10, oxygine::Stage::getStage()->getHeight() - 105);
    }
    addChild(panel);

    oxygine::spButton pButtonFunds = ObjectManager::createButton(tr("Funds"));
    addChild(pButtonFunds);
    pButtonFunds->setPosition(5, 5);
    pButtonFunds->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Funds);
    });
    panel->addItem(pButtonFunds);

    oxygine::spButton pButtonIncome = ObjectManager::createButton(tr("Income"));
    addChild(pButtonIncome);
    pButtonIncome->setPosition(10 + pButtonFunds->getScaledWidth() + pButtonFunds->getX(), 5);
    pButtonIncome->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Income);
    });
    panel->addItem(pButtonIncome);

    oxygine::spButton pButtonBuildings = ObjectManager::createButton(tr("Buildings"));
    addChild(pButtonBuildings);
    pButtonBuildings->setPosition(10 + pButtonIncome->getScaledWidth() + pButtonIncome->getX(), 5);
    pButtonBuildings->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Buildings);
    });
    panel->addItem(pButtonBuildings);

    oxygine::spButton pButtonUnits = ObjectManager::createButton(tr("Units"));
    addChild(pButtonUnits);
    pButtonUnits->setPosition(10 + pButtonBuildings->getScaledWidth() + pButtonBuildings->getX(), 5);
    pButtonUnits->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Units);
    });
    panel->addItem(pButtonUnits);

    oxygine::spButton pButtonPlayerStrength = ObjectManager::createButton(tr("Player Strength"));
    addChild(pButtonPlayerStrength);
    pButtonPlayerStrength->setPosition(10 + pButtonUnits->getScaledWidth() + pButtonUnits->getX(), 5);
    pButtonPlayerStrength->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::PlayerStrength);
    });
    panel->addItem(pButtonPlayerStrength);

    oxygine::spButton pButtonPlayerStatistic = ObjectManager::createButton(tr("Player Statistics"));
    addChild(pButtonPlayerStatistic);
    pButtonPlayerStatistic->setPosition(10 + pButtonPlayerStrength->getScaledWidth() + pButtonPlayerStrength->getX(), 5);
    pButtonPlayerStatistic->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::PlayerStatistics);
    });
    panel->addItem(pButtonPlayerStatistic);

    if (m_pMap->getWinnerTeam() >= 0)
    {
        oxygine::spButton pButtonVictoryRanking = ObjectManager::createButton(tr("Ranking"));
        addChild(pButtonVictoryRanking);
        pButtonVictoryRanking->setPosition(10 + pButtonPlayerStatistic->getScaledWidth() + pButtonPlayerStatistic->getX(), 5);
        pButtonVictoryRanking->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
        {
            emit sigShowGraph(GraphModes::VictoryRanking);
        });
        panel->addItem(pButtonVictoryRanking);

        panel->setContentWidth(pButtonVictoryRanking->getX() + pButtonVictoryRanking->getScaledWidth() + 5);
    }
    else
    {
        panel->setContentWidth(pButtonPlayerStatistic->getX() + pButtonPlayerStatistic->getScaledWidth() + 5);
    }

    style = oxygine::TextStyle(FontManager::getMainFont24());
    m_Textfield = oxygine::spTextField::create();
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    m_Textfield->setStyle(headerStyle);
    if (Settings::getSmallScreenDevice())
    {
        m_Textfield->setVisible(false);
    }
    addChild(m_Textfield);

    m_PlayerSelectPanel = spPanel::create(true, QSize(200, m_pGraphBackground->getScaledHeight()), QSize(200, 100));
    m_PlayerSelectPanel->setPosition(5, m_pGraphBackground->getY());
    if (Settings::getSmallScreenDevice())
    {
        m_PlayerSelectPanel->setX(-m_PlayerSelectPanel->getScaledWidth() + 1);
        auto moveButton = spMoveInButton::create(m_PlayerSelectPanel.get(), m_PlayerSelectPanel->getScaledWidth(), 1, -1);
    }
    addChild(m_PlayerSelectPanel);
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        oxygine::spTextField pTextfield = oxygine::spTextField::create();
        pTextfield->setStyle(style);
        pTextfield->setX(5);
        pTextfield->setY(50 * i + 5);
        pTextfield->setHtmlText((tr("Player: ") + QString::number(i + 1)));
        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setChecked(true);
        pCheckbox->setPosition(15 + pTextfield->getTextRect().width(), pTextfield->getY());
        connect(pCheckbox.get(), &Checkbox::checkChanged, this, [this, i](bool value)
        {
            m_VisiblePlayers[i] = value;
            emit sigShowGraph(m_CurrentGraphMode);
        });
        m_PlayerSelectPanel->addItem(pCheckbox);
        m_PlayerSelectPanel->addItem(pTextfield);
    }
    createStatisticsView();
    // victory score
    qint32 winnerTeam = m_pMap->getWinnerTeam();
    if (winnerTeam >= 0)
    {
        oxygine::TextStyle style48 = oxygine::TextStyle(FontManager::getMainFont48());
        style48.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style48.multiline = true;
        m_VictoryPanel = spPanel::create(true, QSize(oxygine::Stage::getStage()->getWidth() - 10, oxygine::Stage::getStage()->getHeight() - 115),
                                   QSize(oxygine::Stage::getStage()->getWidth() - 10, oxygine::Stage::getStage()->getHeight() - 115));
        m_VictoryPanel->setPosition(5, 5);
        addChild(m_VictoryPanel);
        qint32 startX = oxygine::Stage::getStage()->getWidth() - 700;
        if (startX < 900)
        {
            startX = 900;
        }
        oxygine::spTextField pHeaders = oxygine::spTextField::create();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Speed"));
        pHeaders->setPosition(startX, 5);
        m_VictoryPanel->addItem(pHeaders);
        pHeaders = oxygine::spTextField::create();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Force"));
        pHeaders->setPosition(startX + 160 * 1, 5);
        m_VictoryPanel->addItem(pHeaders);
        pHeaders = oxygine::spTextField::create();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Tech."));
        pHeaders->setPosition(startX + 160 * 2, 5);
        m_VictoryPanel->addItem(pHeaders);
        pHeaders = oxygine::spTextField::create();
        pHeaders->setStyle(style48);
        pHeaders->setHtmlText(tr("Total"));
        pHeaders->setPosition(startX + 160 * 3, 5);
        m_VictoryPanel->addItem(pHeaders);
        m_VictoryPanel->setContentWidth(startX + 200 * 4 + 10);
        qint32 y = 50;
        float scale = 2.0f;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            QVector3D score;
            m_pMap->getGameRecorder()->calculateRang(i, score);
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

            Player* pPlayer = m_pMap->getPlayer(i);
            for (quint8 i2 = 0; i2 < 2; i2++)
            {
                CO* pCO = pPlayer->getCO(i2);
                if (pCO != nullptr)
                {
                    qint32 sentenceWidth = startX - 10;
                    pAnim = pGameManager->getResAnim("dialogfield+mask");
                    if (pAnim != nullptr)
                    {
                        oxygine::spSprite  pTextMask = oxygine::spSprite::create();
                        pTextMask->setWidth(sentenceWidth);
                        pTextMask->setScaleX(static_cast<float>(pTextMask->getScaledWidth()) / static_cast<float>(pAnim->getWidth()));
                        pTextMask->setResAnim(pAnim);
                        pTextMask->setPosition(5, 5 + y);
                        QColor color = pPlayer->getColor();
                        pTextMask->setColor(color);
                        m_VictoryPanel->addItem(pTextMask);
                    }

                    oxygine::spSprite pWinLooseSprite = oxygine::spSprite::create();
                    pAnim = pGameManager->getResAnim("dialogfield");
                    if (pAnim != nullptr)
                    {
                        pWinLooseSprite->setWidth(sentenceWidth);
                        pWinLooseSprite->setScaleX(static_cast<float>(pWinLooseSprite->getScaledWidth()) / static_cast<float>(pAnim->getWidth()));
                        pWinLooseSprite->setResAnim(pAnim);
                        pWinLooseSprite->setPosition(5, 5 + y);
                        m_VictoryPanel->addItem(pWinLooseSprite);
                    }
                    oxygine::spSprite pCOSprite = oxygine::spSprite::create();
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

                    oxygine::TextStyle dialogStyle = oxygine::TextStyle(FontManager::getFont("dialog48"));
                    dialogStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
                    dialogStyle.multiline = true;

                    spLabel winLooseText = spLabel::create(sentenceWidth - 110);
                    style.multiline = true;
                    winLooseText->setStyle(dialogStyle);
                    winLooseText->setHeight(pWinLooseSprite->getScaledHeight());
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
                    m_VictoryTexts[i].append(oxygine::spTextField::create());
                    m_VictoryTexts[i][0]->setPosition(startX, y - 48 * scale);
                    m_VictoryTexts[i][0]->setStyle(style48);
                    m_VictoryTexts[i][0]->setHtmlText("0");
                    m_VictoryPanel->addItem(m_VictoryTexts[i][0]);
                    m_VictoryTexts[i].append(oxygine::spTextField::create());
                    m_VictoryTexts[i][1]->setPosition(startX + 160 * 1, y - 48 * scale);
                    m_VictoryTexts[i][1]->setStyle(style48);
                    m_VictoryTexts[i][1]->setHtmlText("0");
                    m_VictoryPanel->addItem(m_VictoryTexts[i][1]);
                    m_VictoryTexts[i].append(oxygine::spTextField::create());
                    m_VictoryTexts[i][2]->setPosition(startX + 160 * 2, y - 48 * scale);
                    m_VictoryTexts[i][2]->setStyle(style48);
                    m_VictoryTexts[i][2]->setHtmlText("0");
                    m_VictoryPanel->addItem(m_VictoryTexts[i][2]);
                    m_VictoryTexts[i].append(oxygine::spTextField::create());
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
    else
    {
        showGraph(GraphModes::Funds);
    }

    m_pGraphBackground->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            emit sigFinishCurrentGraph();
            pTouchEvent->stopPropagation();
        }
    });
    connect(this, &VictoryMenue::sigFinishCurrentGraph, this, &VictoryMenue::finishGraph, Qt::QueuedConnection);
    pApp->continueRendering();
    Mainapp::getAiProcessPipe().onQuitGame();
    if (Settings::getAiSlave())
    {
        connect(this, &VictoryMenue::sigQuitOnAiPipe, this, &VictoryMenue::quitOnAiPipe, Qt::QueuedConnection);
        emit sigQuitOnAiPipe();
    }
    else if (Mainapp::getSlave())
    {
        multiplayerGameFinished();
    }
    else
    {
        AddScoreToUserdata();
    }
}

void VictoryMenue::quitOnAiPipe()
{
    CONSOLE_PRINT("Killing vicotry menu on ai slave", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    m_pMap->detach();
    oxygine::Actor::detach();
}

void VictoryMenue::multiplayerGameFinished()
{
    spTCPClient pSlaveMasterConnection = Mainapp::getSlaveClient();
    if (pSlaveMasterConnection.get() != nullptr)
    {
        QJsonObject data;
        QString command = NetworkCommands::SLAVEMULTIPLAYERGAMERESULT;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_MATCHTYPE, m_pMap->getGameRules()->getMatchType());
        QJsonArray winnerInfo;
        qint32 winnerTeam = m_pMap->getWinnerTeam();
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            auto * player = m_pMap->getPlayer(i);
            if (player->getControlType() == GameEnums::AiTypes_Human)
            {
                GameEnums::GameResult result = GameEnums::GameResult_Draw;
                if (winnerTeam >= 0)
                {
                    if (m_pMap->getPlayer(i)->getTeam() == winnerTeam)
                    {
                        result = GameEnums::GameResult_Won;
                    }
                    else
                    {
                        result = GameEnums::GameResult_Lost;
                    }
                }
                QJsonObject object;
                object.insert(JsonKeys::JSONKEY_PLAYER, player->getPlayerNameId());
                QJsonArray coInfo;
                for (quint8 i = 0; i < player->getMaxCoCount(); ++i)
                {
                    CO* pCO = player->getCO(i);
                    if (pCO != nullptr)
                    {
                        coInfo.append(pCO->getCoID());
                    }
                }
                object.insert(JsonKeys::JSONKEY_COS, coInfo);
                object.insert(JsonKeys::JSONKEY_GAMERESULT, result);
                winnerInfo.append(object);
            }
        }
        data.insert(JsonKeys::JSONKEY_GAMERESULTARRAY, winnerInfo);
        QJsonDocument doc(data);
        CONSOLE_PRINT("Sending command " + command + "", GameConsole::eDEBUG);
        emit pSlaveMasterConnection->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
    CONSOLE_PRINT("Killing self on slave", GameConsole::eDEBUG);
    connect(&m_despawnSlaveTimer, &QTimer::timeout, this, &VictoryMenue::despawnSlave, Qt::QueuedConnection);
    m_despawnSlaveTimer.start(20000);
}

void VictoryMenue::despawnSlave()
{
    // despawn slave process on finish
    CONSOLE_PRINT("Closing slave cause the game is finished and server didn't respond.", GameConsole::eERROR);
    qint32 ret = m_pMap->getWinnerTeam() + 1;
    QString object = "Init";
    QString func = "onMasterValue";
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter->exists(object, func))
    {
        QJSValue erg = pInterpreter->doFunction(object, func);
        ret = erg.toInt();
    }
    QCoreApplication::exit(ret);
}

void VictoryMenue::recieveServerData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Master server network command received: " + messageType + " for socket " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::DESPAWNSLAVE)
        {
            closeSlave();
        }
        else
        {
            CONSOLE_PRINT("Unknown master server command " + messageType + " received", GameConsole::eDEBUG);
        }
    }
    else
    {
        CONSOLE_PRINT("Unknown master service command " + QString::number(static_cast<qint32>(service)) + " received", GameConsole::eDEBUG);
    }
}

void VictoryMenue::closeSlave()
{
    CONSOLE_PRINT("Closing slave cause all players have disconnected.", GameConsole::eDEBUG);
    QCoreApplication::exit(0);
}

void VictoryMenue::createStatisticsView()
{
    
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_statisticsBox = oxygine::spBox9Sprite::create();
    if (Settings::getSmallScreenDevice())
    {
        m_statisticsBox->setSize(oxygine::Stage::getStage()->getWidth() - 10, oxygine::Stage::getStage()->getHeight() - 110);
        m_statisticsBox->setPosition(5, 10);
    }
    else
    {
        m_statisticsBox->setSize(oxygine::Stage::getStage()->getWidth() - 10, oxygine::Stage::getStage()->getHeight() - 210);
        m_statisticsBox->setPosition(5, 100);
    }
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    m_statisticsBox->setResAnim(pAnim);
    m_statisticsBox->setVisible(false);
    addChild(m_statisticsBox);
    QStringList items;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        items.append(tr("Player ") + QString::number(i + 1));
    }
    m_pStatisticPlayer = spDropDownmenu::create(250, items);
    m_pStatisticPlayer->setTooltipText(tr("Select which player's statistics you'd like to see."));
    m_pStatisticPlayer->setPosition(10, 10);
    m_statisticsBox->addChild(m_pStatisticPlayer);
    connect(m_pStatisticPlayer.get(), &DropDownmenu::sigItemChanged, this, [this](qint32 item)
    {
        showPlayerStatistic(item);
    });
    showPlayerStatistic(0);
}

void VictoryMenue::addShopMoney()
{
    qint32 highestScore = 0;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        auto* inputType = m_pMap->getPlayer(i)->getBaseGameInput();
        if (inputType != nullptr &&
            inputType->getAiType() == GameEnums::AiTypes_Human &&
            m_pMap->getPlayer(i)->getTeam() == m_pMap->getWinnerTeam())
        {
            qint32 score = m_VictoryScores[i].x() + m_VictoryScores[i].y() + m_VictoryScores[i].z();
            if (score > highestScore)
            {
                highestScore = score;
            }
        }
    }
    if (highestScore > 0 && !m_isReplay)
    {
        CONSOLE_PRINT("Adding points to userdata.", GameConsole::eDEBUG);
        Userdata* pUserdata = Userdata::getInstance();
        spDialogValueCounter pDialogValueCounter = spDialogValueCounter::create(pUserdata->getCredtis(), highestScore);
        connect(pDialogValueCounter.get(), &DialogValueCounter::sigFinished, this, &VictoryMenue::onProgressTimerStart, Qt::QueuedConnection);
        addChild(pDialogValueCounter);
        pUserdata->addCredtis(highestScore);
        m_ProgressTimer.stop();
    }
    else
    {
        showGraph(GraphModes::Funds);
    }
}

void VictoryMenue::onProgressTimerStart()
{
    showGraph(GraphModes::Funds);
}

void VictoryMenue::showGraph(VictoryMenue::GraphModes mode)
{
    CONSOLE_PRINT("VictoryMenue::showGraph " + QString::number(static_cast<qint32>(mode)), GameConsole::eDEBUG);
    m_CurrentGraphMode = mode;
    if (m_CurrentGraphMode < GraphModes::Max)
    {
        m_ProgressTimer.stop();
        m_ProgressTimer.start(getStepTime());
        if (!Settings::getSmallScreenDevice())
        {
            m_PlayerSelectPanel->setVisible(true);
        }
        m_pGraphBackground->setVisible(true);
        m_statisticsBox->setVisible(false);
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
            case GraphModes::PlayerStatistics:
            {

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
    else if (m_CurrentGraphMode == GraphModes::VictoryRanking)
    {
        m_ProgressTimer.stop();
        m_ProgressTimer.start(50);
        if (m_VictoryPanel.get() != nullptr)
        {
            m_PlayerSelectPanel->setVisible(false);
            m_pGraphBackground->setVisible(false);
            m_statisticsBox->setVisible(false);
            m_VictoryPanel->setVisible(true);
            m_Textfield->setHtmlText(tr("Victory"));
        }
    }
    else if (m_CurrentGraphMode == GraphModes::PlayerStatistics)
    {
        m_PlayerSelectPanel->setVisible(false);
        m_pGraphBackground->setVisible(false);
        m_statisticsBox->setVisible(true);
        if (m_VictoryPanel.get() != nullptr)
        {
            m_VictoryPanel->setVisible(false);
        }
        m_Textfield->setHtmlText(tr("Player Statistics"));
    }
    m_Textfield->setX(oxygine::Stage::getStage()->getWidth() / 2.0f - m_Textfield->getTextRect().width() / 2.0f);

    
}

void VictoryMenue::exitMenue()
{
    CONSOLE_PRINT("Leaving Victory Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    m_pNetworkInterface.free();
    spCampaign campaign = m_pMap->getSpCampaign();
    if (campaign.get() != nullptr && campaign->getCampaignFinished() == false && !m_isReplay)
    {
        bool multiplayer = m_pNetworkInterface.get() != nullptr;
        oxygine::Stage::getStage()->addChild(spCampaignMenu::create(campaign, multiplayer, true));
    }
    else
    {
        auto window = spMainwindow::create("ui/menu/mainmenu.xml");
        oxygine::Stage::getStage()->addChild(window);
    }
    m_pMap->detach();
    oxygine::Actor::detach();
}

oxygine::spActor VictoryMenue::createLine(QPointF end, qint32 lineWidth, QColor color)
{
    oxygine::spActor pRet = oxygine::spActor::create();
    float m = static_cast<float>(end.y()) / static_cast<float>(end.x());
    qint32 width = lineWidth;
    if (end.y() != 0)
    {
        width = qSqrt(lineWidth * lineWidth * m * m / (m * m + 1));
    }

    // initial rect
    oxygine::spColorRectSprite rect = oxygine::spColorRectSprite::create();
    rect->setColor(color);
    rect->setPosition(-1, 0);
    rect->setSize(lineWidth + 1, lineWidth);
    pRet->addChild(rect);
    // tilted rect
    double angle = 0;
    rect = oxygine::spColorRectSprite::create();
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
    // finishing rect
    rect = oxygine::spColorRectSprite::create();
    rect->setColor(color);
    rect->setPosition(end.x() - lineWidth, end.y());
    rect->setSize(lineWidth + 1, lineWidth);
    pRet->addChild(rect);
    return pRet;
}

void VictoryMenue::updateGraph()
{
    if (m_pMap.get() != nullptr)
    {
        if (m_CurrentGraphMode < GraphModes::Max)
        {
            // make sure we have some graph data to be added :)
            qint32 progress = m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)];
            if (progress < m_pMap->getCurrentDay())
            {
                qint32 nextStep = drawGraphStep(progress);
                m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)] = nextStep;
            }
        }
        else
        {
            if (m_progress < 100)
            {
                m_progress += 2;
                // update values
                for (qint32 i = 0; i < m_VictoryTexts.size(); i++)
                {
                    m_VictoryTexts[i][0]->setHtmlText(QString::number(static_cast<qint32>(m_VictoryScores[i].x() * m_progress / 100)));
                    m_VictoryTexts[i][1]->setHtmlText(QString::number(static_cast<qint32>(m_VictoryScores[i].y() * m_progress / 100)));
                    m_VictoryTexts[i][2]->setHtmlText(QString::number(static_cast<qint32>(m_VictoryScores[i].z() * m_progress / 100)));
                    float sum = m_VictoryScores[i].x() + m_VictoryScores[i].y() +m_VictoryScores[i].z();
                    m_VictoryTexts[i][3]->setHtmlText(QString::number(static_cast<qint32>(sum * m_progress / 100)));
                }
                if (m_progress >= 100)
                {
                    m_progress = 100;
                    // show CO-Rank
                    for (qint32 i = 0; i < m_VictoryTexts.size(); i++)
                    {
                        oxygine::ResAnim* pAnim = nullptr;
                        qint32 sum = static_cast<qint32>(m_VictoryScores[i].x() + m_VictoryScores[i].y() +m_VictoryScores[i].z());
                        GameRecorder::Rang rang = m_pMap->getGameRecorder()->getRank(sum);
                        pAnim = m_pMap->getGameRecorder()->getRankAnim(rang);
                        oxygine::spSprite pRankSprite = oxygine::spSprite::create();
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
    qint32 progress = m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)];
    while (progress < m_pMap->getCurrentDay())
    {
        drawGraphStep(progress);
        m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)]++;
        progress = m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)];
    }
}

qint32 VictoryMenue::drawGraphStep(qint32 progress)
{
    constexpr qint32 lineWidth = 5;
    
    DayToDayRecord* pStartRecord = nullptr;
    DayToDayRecord* pEndRecord = nullptr;
    qint32 endProgress = progress + 1;
    if (m_lineLength < lineWidth * 2.5f)
    {
        endProgress = progress + qCeil(lineWidth * 2.5f / m_lineLength);
    }
    if (m_pMap->getCurrentDay() == 1)
    {
        pStartRecord = m_pMap->getGameRecorder()->getDayRecord(0);
        pEndRecord = m_pMap->getGameRecorder()->getDayRecord(0);
    }
    else if (progress < m_pMap->getCurrentDay() - 1)
    {
        pStartRecord = m_pMap->getGameRecorder()->getDayRecord(progress);
        pEndRecord = m_pMap->getGameRecorder()->getDayRecord(endProgress);
    }
    if (pStartRecord != nullptr &&
        pEndRecord != nullptr)
    {
        m_GraphsProgessLine[static_cast<qint32>(m_CurrentGraphMode)]->setX(endProgress * m_lineLength - m_GraphsProgessLine[static_cast<qint32>(m_CurrentGraphMode)]->getScaledWidth() / 2);
        // add player lines
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            QPointF startPoint(-1, -1);
            QPointF endPoint(-1, -1);

            startPoint.setX(progress * m_lineLength);
            endPoint.setX(endProgress * m_lineLength);

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
                case GraphModes::PlayerStatistics:
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
                startPoint.setY(m_pGraphBackground->getScaledHeight() - startPoint.y() * m_pGraphBackground->getScaledHeight());
                endPoint.setY(m_pGraphBackground->getScaledHeight() - endPoint.y() * m_pGraphBackground->getScaledHeight());
                oxygine::spActor line = createLine(endPoint - startPoint, lineWidth, m_pMap->getPlayer(i)->getColor());
                line->setPosition(startPoint.x(), startPoint.y() - lineWidth / 2);
                m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][i]->addChild(line);
            }
            drawPlayerEvents(pStartRecord, i, startPoint, progress);
        }
    }
    return endProgress;
}

void VictoryMenue::drawPlayerEvents(DayToDayRecord* pStartRecord, qint32 player,
                              QPointF startPoint, qint32 progress)
{
    GameManager* pGameManager = GameManager::getInstance();
    for (qint32 event = 0; event < pStartRecord->getEventRecordCount(); event++)
    {
        SpecialEvent* pEvent = pStartRecord->getSpecialEvent(event);
        if (pEvent->getOwner() == player)
        {
            oxygine::spSprite pSprite = oxygine::spSprite::create();
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
                    QColor color = m_pMap->getPlayer(player)->getColor();
                    pSprite->setColor(color.red(), color.green(), color.blue(), color.alpha());
                    break;
                }
                case GameEnums::GameRecord_SpecialEvents_HQLost:
                {
                    pAnim = pGameManager->getResAnim("hq+lost");
                    QColor color = m_pMap->getPlayer(player)->getColor();
                    pSprite->setColor(color.red(), color.green(), color.blue(), color.alpha());
                    break;
                }
            }
            pSprite->setResAnim(pAnim);
            if (pAnim != nullptr)
            {
                pSprite->setScale(2.0f);
                if (startPoint.y() < 0)
                {
                    if (progress >= 0)
                    {
                        oxygine::spActor pActor = m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][player]->getLastChild();
                        if (pActor.get() != nullptr)
                        {
                            startPoint.setY(pActor->getY());
                        }
                    }
                }
                pSprite->setPosition(startPoint.x() - pAnim->getWidth(), startPoint.y() - pAnim->getHeight());
                m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][player]->addChild(pSprite);
            }
        }
    }
}

qint32 VictoryMenue::getStepTime()
{
    qint32 stepTime = static_cast<qint32>(15.0 * (20.0 - qExp(m_pMap->getCurrentDay() / 10.0)));
    if (m_pMap->getCurrentDay() < 5)
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
    CONSOLE_PRINT("VictoryMenue::AddScoreToUserdata", GameConsole::eDEBUG);
    
    QString path = m_pMap->getMapPath();
    if (!path.isEmpty() && m_pMap->getWinnerTeam() >= 0 && !m_isReplay)
    {
        qint32 playerCount = m_pMap->getPlayerCount();
        qint32 bestPlayer = -1;
        qint32 bestScore = -1;
        for (qint32 i = 0; i < playerCount; ++i)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            auto* input = pPlayer->getBaseGameInput();
            if (input != nullptr &&
                input->getAiType() == GameEnums::AiTypes::AiTypes_Human &&
                pPlayer->getTeam() == m_pMap->getWinnerTeam())
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
            Player* pPlayer = m_pMap->getPlayer(bestPlayer);
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
    CONSOLE_PRINT("VictoryMenue::onEnter", GameConsole::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "onVictory";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

void VictoryMenue::showPlayerStatistic(qint32 player)
{
    CONSOLE_PRINT("VictoryMenue::showPlayerStatistic for " + QString::number(player), GameConsole::eDEBUG);
    if (m_statisticsView.get() != nullptr)
    {
        m_statisticsView->detach();
    }
    
    const auto & data = m_pMap->getGameRecorder()->getPlayerDataRecords();
    if (player >= 0 && player < data.size())
    {
        const auto & playerdata = data[player];
        if (Settings::getSmallScreenDevice())
        {
            m_statisticsView = spUnitStatisticView::create(playerdata, oxygine::Stage::getStage()->getWidth() - 30, oxygine::Stage::getStage()->getHeight() - 180, m_pMap->getPlayer(player), m_pMap.get());
        }
        else
        {
            m_statisticsView = spUnitStatisticView::create(playerdata, oxygine::Stage::getStage()->getWidth() - 30, oxygine::Stage::getStage()->getHeight() - 280, m_pMap->getPlayer(player), m_pMap.get());
        }
        m_statisticsView->setPosition(10, 60);
        m_statisticsBox->addChild(m_statisticsView);
    }
}
