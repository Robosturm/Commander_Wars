#include "ai/heavyai.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

// code for debugging and neuro evolution is here

QString HeavyAi::getNeuralNetworkName(qint32 network)
{
    if (network >= 0 && network < m_neuralNetworks.size())
    {
        return m_neuralNetworks[network]->getNetworkName();
    }
    else
    {
        CONSOLE_PRINT("HeavyAi::getNeuralNetworkName invalid index " + QString::number(network), Console::eDEBUG);
    }
    return "";
}

void HeavyAi::mutateNeuralNetwork(qint32 network, double mutationChance, double mutationRate)
{
    if (network >= 0 && network < m_neuralNetworks.size())
    {
        m_neuralNetworks[network]->mutateAllWeights(mutationChance, mutationRate);
    }
    else
    {
        CONSOLE_PRINT("HeavyAi::mutateNeuralNetwork invalid index " + QString::number(network), Console::eDEBUG);
    }
}

void HeavyAi::combineAi(QStringList aisToUse)
{
    CONSOLE_PRINT("HeavyAi::combineAi", Console::eDEBUG);
    for (qint32 i = 0; i < NeuralNetworksMax; ++i)
    {
        qint32 item = GlobalUtils::randInt(0, aisToUse.length() - 1);
        QString netName = NeuralNetworkNames[i];
        QString targetName = QString("resources/") + NeuralNetworkPath + netName + m_aiName + NeuralNetworkFileEnding;
        QFile::remove(targetName);
        QFile::copy(QString("resources/") + NeuralNetworkPath + netName + aisToUse[item] + NeuralNetworkFileEnding, targetName);
    }
}

void HeavyAi::saveNeuralNetwork(qint32 network)
{
    if (network >= 0 && network < m_neuralNetworks.size())
    {
        QFile file(QString("resources/") + NeuralNetworkPath + NeuralNetworkNames[network] + m_aiName + NeuralNetworkFileEnding);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        m_neuralNetworks[network]->serializeObject(stream);
    }
    else
    {
        CONSOLE_PRINT("HeavyAi::saveNeuralNetwork invalid index " + QString::number(network), Console::eDEBUG);
    }
}

void HeavyAi::toggleAiPause()
{
    m_pause = !m_pause;
}

void HeavyAi::showFrontMap()
{
    m_InfluenceFrontMap.show();
}

void HeavyAi::showFrontLines()
{
    m_InfluenceFrontMap.showFrontlines();
}

void HeavyAi::showUnitPfs(bool enemy, qint32 index)
{
    if (enemy)
    {
        if (index >= 0 && index < m_enemyUnits.size())
        {
            m_InfluenceFrontMap.showPfs(m_enemyUnits[index].m_pPfs.get());
        }
    }
    else
    {
        if (index >= 0 && index < m_ownUnits.size())
        {
            m_InfluenceFrontMap.showPfs(m_ownUnits[index].m_pPfs.get());
        }
    }
}

void HeavyAi::hideFrontMap()
{
    m_InfluenceFrontMap.hide();
}
