#include "ai/neuralnetwork/optimizer/optimizer.h"
#include "coreengine/console.h"

Optimizer::Optimizer(double learningRate)
    : m_learningRate(learningRate)
{
}

Optimizer::~Optimizer()
{
}

void Optimizer::minimize()
{
    Console::print("not implemented", Console::eDEBUG);
}

void Optimizer::setDataset(spDataset dataset)
{
    m_d = dataset;
}

void Optimizer::setNeuralNetwork(spNeuralNetwork net)
{
    m_n = net;
    if (m_n.get() != nullptr)
    {
        m_n->setLearningRate(m_learningRate);
    }
}


double Optimizer::getScore(Dataset::Datatype d, qint32 limit)
{
    return m_n->predictAllForScore(m_d, d, limit);
}

