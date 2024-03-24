#include "ai/heavyai/heavyaitrainingdatagenerator.h"
#include "ai/heavyai/heavyAiSharedData.h"
#include "ai/heavyai/situationevaluator.h"

#include "game/gamemap.h"
#include "game/player.h"

#include "coreengine/settings.h"

HeavyAiTrainingDataGenerator::HeavyAiTrainingDataGenerator(GameMap* pMap, const QString & type, const QString & trainingFile)
    : HeavyAi(pMap, type, GameEnums::AiTypes_Heavy),
    m_trainingDataFile(trainingFile),
    m_stream(&m_trainingDataFile)
{
    m_trainingDataFile.open(QIODevice::Append);
}

void HeavyAiTrainingDataGenerator::init(BaseGamemenu* pMenu)
{
    if (!m_initDone)
    {
        m_pPlayer =  m_pMap->getPlayer(0);
        m_evaluator = MemoryManagement::create<SituationEvaluator>(m_pPlayer);
    }
}

void HeavyAiTrainingDataGenerator::addCurrentSituationTrainingData()
{
    if (Settings::getInstance()->getHeavyAiTrainingStartDay() <= m_pMap->getCurrentDay())
    {
        Player* pCurrentPlayer = m_pMap->getCurrentPlayer();
        HeavyAiSharedData::SituationOutput result = HeavyAiSharedData::SituationOutput::SituationOutput_Draw;
        if (m_winnerTeam >= 0)
        {
            if (pCurrentPlayer->getTeam() == m_winnerTeam)
            {
                result = HeavyAiSharedData::SituationOutput::SituationOutput_Win;
            }
            else
            {
                result = HeavyAiSharedData::SituationOutput::SituationOutput_Lost;
            }
        }
        m_pPlayer = pCurrentPlayer;
        m_evaluator->setOwner(m_pPlayer);
        spQmlVectorUnit pUnits = pCurrentPlayer->getSpUnits();
        spQmlVectorUnit pEnemyUnits = pCurrentPlayer->getSpEnemyUnits();
        updateUnitCache(pUnits);
        updateUnitCache(pEnemyUnits);
        for (auto & pUnit : pUnits->getVector())
        {
            m_evaluator->updateInputVector(m_pMap, pUnit->getPosition(), true);
            QString line = m_evaluator->getInputVector() + QString::number(static_cast<qint32>(result));
            m_stream << line << "\n";
        }
    }
}

qint32 HeavyAiTrainingDataGenerator::getWinnerTeam() const
{
    return m_winnerTeam;
}

void HeavyAiTrainingDataGenerator::setWinnerTeam(qint32 newWinnerTeam)
{
    m_winnerTeam = newWinnerTeam;
}
