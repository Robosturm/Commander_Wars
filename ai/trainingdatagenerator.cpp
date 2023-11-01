#include "ai/trainingdatagenerator.h"
#include "ai/coreai.h"
#include "ai/heavyai/situationevaluator.h"
#include "coreengine/memorymanagement.h"
#include "coreengine/settings.h"
#include "game/gamemap.h"
#include "game/gameaction.h"

TrainingDataGenerator::TrainingDataGenerator(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("TrainingDataGenerator");
#endif
    Interpreter::setCppOwnerShip(this);
    init();
}

void TrainingDataGenerator::init()
{
    m_evaluators.clear();
    m_data.clear();
    if (Settings::getInstance()->getCreateAiTrainingData())
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
        {
            m_evaluators.push_back(MemoryManagement::create<SituationEvaluator>(m_pMap->getPlayer(i)));
            m_data.push_back(QStringList());
        }
    }
}

void TrainingDataGenerator::onActionDone(GameAction* pAction)
{
    if (Settings::getInstance()->getCreateAiTrainingData())
    {
        if (pAction->getActionID() == CoreAI::ACTION_BUILD_UNITS)
        {

        }
        else if (pAction->getMovePathLength() > 0)
        {
            qint32 player = m_pMap->getCurrentPlayer()->getPlayerID();
            m_evaluators[player]->updateInputVector(m_pMap, pAction->getActionTarget(), true);
            m_data[player].append(m_evaluators[player]->getInputVector());
        }
    }
}

void TrainingDataGenerator::saveDataToFile()
{
    saveDataToFile(m_pMap->getMapName() + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss") + ".csv");
}

void TrainingDataGenerator::saveDataToFile(const QString & filepath)
{
    if (Settings::getInstance()->getCreateAiTrainingData())
    {
        qint32 winner = m_pMap->getWinnerTeam();
        if (winner >= 0 || m_pMap->getGameRules()->getDrawVotingResult() == GameEnums::DrawVoting_Yes)
        {
            QFile file("situationResults_" + filepath);
            file.open(QFile::OpenModeFlag::WriteOnly);
            QTextStream stream(&file);
            for (qint32 i = 0; i < m_data.size(); ++i)
            {
                QString output;
                if (winner < 0)
                {
                    output = "0";
                }
                else if (winner == m_pMap->getPlayer(i)->getTeam())
                {
                    output = "1";
                }
                else
                {
                    output = "-1";
                }
                for (qint32 i2 = 0; i2 < m_data.size(); ++i2)
                {
                    stream << m_data[i][i2] << ";" << output;
                }
            }
        }
    }
}

void TrainingDataGenerator::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void TrainingDataGenerator::serializeObject(QDataStream& pStream, bool forHash) const
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_data.size());
    for (auto & data : m_data)
    {
        pStream << static_cast<qint32>(data.size());
        for (auto & item : data)
        {
            pStream << item;
        }
    }
}

void TrainingDataGenerator::deserializeObject(QDataStream& pStream)
{
    init();
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        m_data.push_back(QStringList());
        qint32 size2 = 0;
        pStream >> size2;
        for (qint32 i2 = 0; i2 < size2; ++i2)
        {
            QString item;
            pStream >> item;
            m_data[i].push_back(item);
        }
    }
}
