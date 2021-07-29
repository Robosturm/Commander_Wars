#include "ai/neuralnetwork/neural/edge.h"
#include "ai/neuralnetwork/neural/neuron.h"
#include "ai/neuralnetwork/neural/layer.h"
#include "ai/neuralnetwork/neural/neuralnetwork.h"

Edge::Edge(Neuron *nextNeuron, Neuron* previousNeuron, double weight)
    : m_nextNeuron(nextNeuron),
      m_previousNeuron(previousNeuron),
      m_weight(weight)
{
}

Neuron* Edge::nextNeuron() const
{
    return m_nextNeuron;
}

Neuron* Edge::previousNeuron() const
{
    return m_previousNeuron;
}

double Edge::weight() 
{
    return m_weight;
}

void Edge::propagate(double neuron_output)
{
    m_nextNeuron->addAccumulated(neuron_output * m_weight);
}

void Edge::alterWeight(double w)
{
    m_weight = w;
}

void Edge::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_weight;
}

void Edge::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_weight;
}
