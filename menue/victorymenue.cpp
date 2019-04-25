#include <QtMath>

#include "victorymenue.h"

#include "menue/mainwindow.h"

#include "game/gamemap.h"
#include "game/gamerecording/gamerecorder.h"
#include "game/player.h"
#include "game/co.h"

#include "resource_management/gamemanager.h"
#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

VictoryMenue::VictoryMenue()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Victory Menue", Console::eDEBUG);
    GameMap* pMap = GameMap::getInstance();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
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
    pApp->getAudioThread()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioThread()->playRandom();

    GameManager* pGameManager = GameManager::getInstance();
    m_pGraphBackground = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("graph");
    m_pGraphBackground->setResAnim(pAnim);
    qint32 widthCount = (pApp->getSettings()->getWidth() - 310) / static_cast<qint32>(pAnim->getWidth());
    qint32 heigthCount = (pApp->getSettings()->getHeight() - 40 - 105 - 100 - 20) / static_cast<qint32>(pAnim->getHeight());
    m_pGraphBackground->setSize(widthCount * pAnim->getWidth(), heigthCount * pAnim->getHeight());
    m_pGraphBackground->setVerticalMode(oxygine::Box9Sprite::TILING);
    m_pGraphBackground->setHorizontalMode(oxygine::Box9Sprite::TILING);
    m_pGraphBackground->setColor(255, 255, 255, 200);
    m_pGraphBackground->setGuides(0, pAnim->getWidth(), 0, pAnim->getHeight());
    m_pGraphBackground->setPosition(pApp->getSettings()->getWidth() - 10 - m_pGraphBackground->getWidth(), 110);
    addChild(m_pGraphBackground);
    qint32 graphDays = m_pGraphBackground->getWidth() / 100;
    for (qint32 i = 0; i < graphDays; i++)
    {
        oxygine::spTextField pDayText = new oxygine::TextField();
        pDayText->setY(m_pGraphBackground->getHeight());
        pDayText->setX(i * 100);
        pDayText->setStyle(style);
        qint32 day = static_cast<qint32>((i * pMap->getCurrentDay()) / graphDays) + 1;
        pDayText->setText(QString::number(day).toStdString().c_str());
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
    pButtonExit->setPosition(pApp->getSettings()->getWidth() - pButtonExit->getWidth() - 10,
                             pApp->getSettings()->getHeight() - pButtonExit->getHeight() - 10);
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
    }
    // find max values for each graph here
    for (qint32 i = 0; i < pMap->getCurrentDay(); i++)
    {
        DayToDayRecord* pDayRecord = pMap->getGameRecorder()->getDayRecord(i);
        if (pDayRecord != nullptr)
        {
            for (qint32 i2 = 0; i2 < pMap->getPlayerCount(); i2++)
            {
                qint32 fonds = pDayRecord->getPlayerRecord(i2)->getFonds();
                qint32 income = pDayRecord->getPlayerRecord(i2)->getIncome();
                qint32 buildings = pDayRecord->getPlayerRecord(i2)->getBuildings();
                qint32 units = pDayRecord->getPlayerRecord(i2)->getUnits();
                qint32 playerStrength = pDayRecord->getPlayerRecord(i2)->getPlayerStrength();
                if (fonds > m_GraphMaxValues[static_cast<qint32>(GraphModes::Fonds)])
                {
                    m_GraphMaxValues[static_cast<qint32>(GraphModes::Fonds)] = fonds;
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
        lineLength = static_cast<qint32>(m_pGraphBackground->getWidth() / (pMap->getCurrentDay() - 1));
    }
    else
    {
        lineLength = static_cast<qint32>(m_pGraphBackground->getWidth());
    }

    m_ProgressTimer.setSingleShot(false);
    m_ProgressTimer.start(500);
    connect(&m_ProgressTimer, &QTimer::timeout, this, &VictoryMenue::updateGraph, Qt::QueuedConnection);

    spPanel panel = new Panel(true, QSize(pApp->getSettings()->getWidth() - pButtonExit->getWidth() - 30, 105), QSize(pApp->getSettings()->getWidth() - pButtonExit->getX() - 20, 40));
    panel->setPosition(10, pApp->getSettings()->getHeight() - 105);
    addChild(panel);

    oxygine::spButton pButtonFonds = ObjectManager::createButton(tr("Fonds"));
    pButtonFonds->attachTo(this);
    pButtonFonds->setPosition(5, 5);
    pButtonFonds->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGraph(GraphModes::Fonds);
    });
    panel->addItem(pButtonFonds);

    oxygine::spButton pButtonIncome = ObjectManager::createButton(tr("Income"));
    pButtonIncome->attachTo(this);
    pButtonIncome->setPosition(10 + pButtonFonds->getWidth() + pButtonFonds->getX(), 5);
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
    m_Textfield->setStyle(style);
    m_Textfield->setY(5);
    m_Textfield->setScale(3.0f);
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
        pTextfield->setText((tr("Player: ") + QString::number(i + 1)).toStdString().c_str());
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
    if (pMap->getWidth() >= 0)
    {
        m_VictoryPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() ,m_pGraphBackground->getHeight()),
                                   QSize(pApp->getSettings()->getWidth() ,m_pGraphBackground->getHeight()));
        m_VictoryPanel->setPosition(5, 5);
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            QVector3D score;
            pMap->getGameRecorder()->calculateRang(i, score);
            m_VictoryScores.append(score);
        }

    }

    showGraph(GraphModes::Fonds);
}

void VictoryMenue::showGraph(VictoryMenue::GraphModes mode)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_CurrentGraphMode = mode;
    if (m_CurrentGraphMode < GraphModes::Max)
    {
        m_PlayerSelectPanel->setVisible(true);
        m_pGraphBackground->setVisible(true);
        m_VictoryPanel->setVisible(false);
        for (qint32 i = 0; i < m_YGraphItems.size(); i++)
        {
            m_YGraphItems[i]->setText(QString::number(static_cast<qint32>(m_GraphMaxValues[static_cast<qint32>(m_CurrentGraphMode)]
                                                      * (1.0f - i / static_cast<float>(m_YGraphItems.size())))).toStdString().c_str());
        }

        switch (m_CurrentGraphMode)
        {
            case GraphModes::Fonds:
            {
                m_Textfield->setText(tr("Fonds").toStdString().c_str());
                break;
            }
            case GraphModes::Income:
            {
                m_Textfield->setText(tr("Income").toStdString().c_str());
                break;
            }
            case GraphModes::Buildings:
            {
                m_Textfield->setText(tr("Buildings").toStdString().c_str());
                break;
            }
            case GraphModes::Units:
            {
                m_Textfield->setText(tr("Units").toStdString().c_str());
                break;
            }
            case GraphModes::PlayerStrength:
            {
                m_Textfield->setText(tr("Player Strength").toStdString().c_str());
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
        m_PlayerSelectPanel->setVisible(false);
        m_pGraphBackground->setVisible(false);
        m_VictoryPanel->setVisible(true);
        m_Textfield->setText(tr("Victory").toStdString().c_str());

    }
    m_Textfield->setX(pApp->getSettings()->getWidth() / 2.0f - m_Textfield->getTextRect().getWidth() / 2.0f);

    pApp->continueThread();
}

void VictoryMenue::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Victory Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    GameMap::getInstance()->deleteMap();
    oxygine::Actor::detach();
    pApp->continueThread();
}

oxygine::spPolygon VictoryMenue::createLine(QPointF end, qint32 lineWidth, QColor color)
{
    oxygine::spPolygon poly = new oxygine::Polygon();
    poly->setResAnim(ObjectManager::getInstance()->getResAnim("line"));
    oxygine::vertexPCT2* vertices = new oxygine::vertexPCT2[8];
    // convert color to agbr value who smoked to much here?
    quint32 colorValue = static_cast <quint32>(color.alpha()) * 256u * 256u * 256u + static_cast <quint32>(color.blue()) * 256u * 256u +
                         static_cast <quint32>(color.green()) * 256u + static_cast <quint32>(color.red());

    float m = static_cast<float>(end.y()) / static_cast<float>(end.x());
    qint32 width = lineWidth;
    if (end.y() != 0)
    {
        width = qSqrt(lineWidth * lineWidth * m * m / (m * m + 1));
    }

    if (end.y() < 0)
    {
        // first strip
        vertices[0].color = colorValue;
        vertices[0].x = 0;
        vertices[0].y = lineWidth;
        vertices[0].z = 0;
        vertices[0].u = 0;
        vertices[0].v = 1;
        vertices[1].color = colorValue;
        vertices[1].x = 0;
        vertices[1].y = 0;
        vertices[1].z = 0;
        vertices[1].u = 0;
        vertices[1].v = 0;
        vertices[2].color = colorValue;
        vertices[2].x = width;
        vertices[2].y = lineWidth;
        vertices[2].z = 0;
        vertices[2].u = 1;
        vertices[2].v = 1;
        vertices[3].color = colorValue;
        vertices[3].x = end.x() - width;
        vertices[3].y = end.y();
        vertices[3].z = 0;
        vertices[3].u = 1;
        vertices[3].v = 0;
        // second strip
        vertices[4].color = colorValue;
        vertices[4].x = width;
        vertices[4].y = lineWidth;
        vertices[4].z = 0;
        vertices[4].u = 0;
        vertices[4].v = 1;
        vertices[5].color = colorValue;
        vertices[5].x = end.x() - width;
        vertices[5].y = end.y();
        vertices[5].z = 0;
        vertices[5].u = 0;
        vertices[5].v = 0;
        vertices[6].color = colorValue;
        vertices[6].x = end.x();
        vertices[6].y = end.y() + lineWidth;
        vertices[6].z = 0;
        vertices[6].u = 1;
        vertices[6].v = 1;
        vertices[7].color = colorValue;
        vertices[7].x = end.x();
        vertices[7].y = end.y();
        vertices[7].z = 0;
        vertices[7].u = 1;
        vertices[7].v = 0;
    }
    else
    {
        // first strip
        vertices[0].color = colorValue;
        vertices[0].x = end.x();
        vertices[0].y = end.y();
        vertices[0].z = 0;
        vertices[0].u = 1;
        vertices[0].v = 1;
        vertices[1].color = colorValue;
        vertices[1].x = 0;
        vertices[1].y = lineWidth;
        vertices[1].z = 0;
        vertices[1].u = 0;
        vertices[1].v = 1;
        vertices[2].color = colorValue;
        vertices[2].x = width;
        vertices[2].y = 0;
        vertices[2].z = 0;
        vertices[2].u = 1;
        vertices[2].v = 0;
        vertices[3].color = colorValue;
        vertices[3].x = 0;
        vertices[3].y = 0;
        vertices[3].z = 0;
        vertices[3].u = 0;
        vertices[3].v = 0;
        // second strip
        vertices[4].color = colorValue;
        vertices[4].x = end.x();
        vertices[4].y = end.y() + lineWidth;
        vertices[4].z = 0;
        vertices[4].u = 1;
        vertices[4].v = 1;
        vertices[5].color = colorValue;
        vertices[5].x = end.x() - width;
        vertices[5].y = end.y() + lineWidth;
        vertices[5].z = 0;
        vertices[5].u = 0;
        vertices[5].v = 1;
        vertices[6].color = colorValue;
        vertices[6].x = end.x();
        vertices[6].y = end.y();
        vertices[6].z = 0;
        vertices[6].u = 1;
        vertices[6].v = 0;
        vertices[7].color = colorValue;
        vertices[7].x = 0;
        vertices[7].y = lineWidth;
        vertices[7].z = 0;
        vertices[7].u = 0;
        vertices[7].v = 0;
    }
    poly->setVertices(vertices, sizeof(oxygine::vertexPCT2) * 8, oxygine::vertexPCT2::FORMAT, true);
    return poly;
}

void VictoryMenue::updateGraph()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameManager* pGameManager = GameManager::getInstance();
    if (m_CurrentGraphMode < GraphModes::Max)
    {
        GameMap* pMap = GameMap::getInstance();
        // make sure we have some graph data to be added :)
        qint32 progress = m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)];
        if (progress < pMap->getCurrentDay())
        {
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
                // add player lines
                for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
                {
                    QPointF startPoint(-1, -1);
                    QPointF endPoint(-1, -1);

                    startPoint.setX(progress * lineLength);
                    endPoint.setX((progress + 1) * lineLength);

                    switch (m_CurrentGraphMode)
                    {
                        case GraphModes::Fonds:
                        {
                            startPoint.setY(pStartRecord->getPlayerRecord(i)->getFonds());
                            endPoint.setY(pEndRecord->getPlayerRecord(i)->getFonds());
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
                        oxygine::spPolygon line = createLine(endPoint - startPoint, lineWidth, pMap->getPlayer(i)->getColor());
                        line->setPosition(startPoint.x(), startPoint.y() - lineWidth / 2);
                        m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][i]->addChild(line);
                    }
                    for (qint32 event = 0; event < pStartRecord->getEventRecordCount(); event++)
                    {
                        SpecialEvent* pEvent = pStartRecord->getSpecialEvent(event);
                        if (pEvent->getPlayer() == i)
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
                            pSprite->setPosition(startPoint.x() - pAnim->getWidth(), startPoint.y() - pAnim->getHeight());
                            m_PlayerGraphs[static_cast<qint32>(m_CurrentGraphMode)][i]->addChild(pSprite);
                        }
                    }
                }
            }
            m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)]++;
            if (m_GraphProgress[static_cast<qint32>(m_CurrentGraphMode)] >= pMap->getCurrentDay())
            {
                // auto skip to next mode
                m_CurrentGraphMode = static_cast<GraphModes>(static_cast<qint32>(m_CurrentGraphMode) + 1);
                if (m_CurrentGraphMode == GraphModes::Max)
                {
                    if (pMap->getWinnerTeam() >= 0)
                    {
                        m_CurrentGraphMode = GraphModes::VictoryRanking;
                    }
                    else
                    {
                        m_CurrentGraphMode = GraphModes::PlayerStrength;
                    }
                }
                showGraph(m_CurrentGraphMode);
            }
        }
    }
    pApp->continueThread();
}
