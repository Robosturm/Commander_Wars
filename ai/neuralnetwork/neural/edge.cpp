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

void Edge::shiftWeight(double dw)
{
    dw *= m_previousNeuron->getLayer()->getNet()->getLearningRate();
    m_weight += dw;
    m_last_shift = dw;
}

void Edge::resetLastShift()
{
    m_weight -= m_last_shift;
}

double Edge::getLastShift() const
{
    return m_last_shift;
}


double Edge::backpropagationMemory() const
{
    return m_backpropagation_memory;
}

void Edge::setBackpropagationMemory(double v)
{
    m_backpropagation_memory = v;
}

void Edge::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_weight;
    pStream << m_last_shift;
    pStream << m_backpropagation_memory;
}

void Edge::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_weight;
    pStream >> m_last_shift;
    pStream >> m_backpropagation_memory;
}
