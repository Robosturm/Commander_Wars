#include "ai/neuralnetwork/neural/edge.h"
#include "ai/neuralnetwork/neural/neuron.h"
#include "ai/neuralnetwork/neural/layer.h"
#include "ai/neuralnetwork/neural/neuralnetwork.h"

Edge::Edge(Neuron *n, Neuron* nb, double w)
    : m_n(n),
      m_nb(nb),
      m_w(w)
{
}

Neuron* Edge::neuron() const
{
    return m_n;
}

Neuron* Edge::neuronb() const
{
    return m_nb;
}

double Edge::weight() 
{
    return m_w;
}

double* Edge::weightP()
{
    return &m_w;
}

void Edge::propagate(double neuron_output)
{
    neuron()->addAccumulated(neuron_output * weight());
}

void Edge::alterWeight(double w)
{
    m_w = w;
}

void Edge::shiftWeight(double dw)
{
    dw *= m_nb->getLayer()->getNet()->getLearningRate();
    m_w += dw;
    m_last_shift = dw;
}

void Edge::resetLastShift()
{
    m_w -= m_last_shift;
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

