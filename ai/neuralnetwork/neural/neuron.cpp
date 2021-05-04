#include "ai/neuralnetwork/neural/neuron.h"
#include "ai/neuralnetwork/neural/layer.h"
#include "ai/neuralnetwork/neural/neuralnetwork.h"
#include "coreengine/globalutils.h"

Neuron::Neuron(qint32 id_neuron, Layer* layer, ActivationFunction function, bool is_bias)
    : m_layer(layer),
      m_id_neuron(id_neuron),
      m_activation_function(function),
      m_is_bias(is_bias)
{	
}

Neuron::~Neuron()
{
}

void Neuron::trigger()
{
    for (spEdge & e : m_next)
    {
        e->propagate(output());
    }

}

double Neuron::in()
{
    return m_accumulated;
}

double Neuron::output()
{
    if (m_is_bias)
    {
        return 1;
    }
    else if(m_layer->getType() == Layer::LayerType::INPUT)
    {
        return outputRaw();
    }
    else if (m_activation_function == ActivationFunction::LINEAR)
    {
        return m_accumulated;
    }
    else if(m_activation_function == ActivationFunction::RELU)
    {
        return GlobalUtils::relu(m_accumulated);
    }
    else if (m_activation_function == ActivationFunction::SIGMOID)
    {
        return GlobalUtils::sigmoid(m_accumulated);
    }
    return outputRaw();
}

double Neuron::outputDerivative()
{
    if (m_activation_function == ActivationFunction::LINEAR)
    {
        return 1;
    }
    else if (m_activation_function == ActivationFunction::RELU)
    {
        return GlobalUtils::relu_derivative(output());
    }
    else if (m_activation_function == ActivationFunction::SIGMOID)
    {
        return GlobalUtils::sigmoid_derivative(outputRaw());
    }
    return m_accumulated;
}

double Neuron::outputRaw()
{
    return m_accumulated;
}

void Neuron::clean()
{
    setAccumulated(0);
}

void Neuron::addAccumulated(double v)
{
    setAccumulated(m_accumulated + v);
}

void Neuron::addNext(spNeuron n)
{
    m_next.push_back(spEdge::create(n.get(), this, GlobalUtils::randDouble(-5, 5)));
    n->addPrevious(m_next.back());
}

void Neuron::addPrevious(spEdge e)
{
    m_previous.push_back(e);
}

int Neuron::getNeuronId() const
{
    return m_id_neuron;
}

void Neuron::setAccumulated(double v)
{

    m_accumulated = v;
}

void Neuron::alterWeights(const QVector<double>& weights)
{
    for(qint32 i_edge = 0; i_edge < weights.size(); ++i_edge)
    {
        m_next[i_edge]->alterWeight(weights[i_edge]);
    }
}

QVector<double> Neuron::getWeights()
{
    QVector<double> w;
    w.reserve(m_next.size());
    for (qint32 i_edge = 0; i_edge < m_next.size(); ++i_edge)
    {
        w.push_back(m_next[i_edge]->weight());
    }
    return w;
}

QVector<spEdge> Neuron::getEdges()
{
    QVector<spEdge> w;
    w.reserve(m_next.size());
    for (qint32 i_edge = 0; i_edge < m_next.size(); ++i_edge)
    {
        w.push_back(m_next[i_edge]);
    }
    return w;
}

void Neuron::randomizeAllWeights(double abs_value)
{
    for(spEdge & e : m_next)
    {
        e->alterWeight(GlobalUtils::randDouble(-abs_value, abs_value));
    }
}


QString Neuron::toString()
{
    QString weights;
    for(spEdge & e : m_next)
    {
        weights.append(QString::number(e->weight()) + ",");
    }
    QString str =  "[" +  QString::number(m_layer->getId()) + "," + QString::number(m_id_neuron) + "]" + "("+ weights +")";
    return str;
}

void Neuron::shiftWeights(float range)
{
    for (spEdge & e : m_next)
    {
        e->alterWeight(e->weight() + GlobalUtils::randFloat(-range, range));
    }
}

void Neuron::shiftBackWeights(const QVector<double>& w)
{
    for (qint32 i = 0; i < m_previous.size(); i++)
    {
        m_previous[i]->shiftWeight(w[i]);
    }
}

//gradient descent
QVector<double> Neuron::getBackpropagationShifts(const QVector<double>& target)
{
    QVector<double> dw(m_previous.size(),0);
    if (m_layer->getType() == Layer::LayerType::OUTPUT)
    {
        double d1 = output() - target[this->getNeuronId()];
        double d2 = outputDerivative();
        for (qint32 i = 0; i < m_previous.size(); ++i)
        {
            dw[i] = (-d1 * d2 * m_previous[i]->neuronb()->output());
            m_previous[i]->setBackpropagationMemory(d1 * d2);
        }
    }
    else
    {
        float d = 0;
        for (qint32 i = 0; i < m_next.size(); i++)
        {
            d += m_next[i]->backpropagationMemory() * m_next[i]->weight();
        }
        d *= outputDerivative();
        for (qint32 i = 0; i < m_previous.size(); i++)
        {
            m_previous[i]->setBackpropagationMemory(d);
            dw[i] = -d * m_previous[i]->neuronb()->output();
        }
    }
    return dw;
}

bool Neuron::isBias() const
{
    return m_is_bias;
}

void Neuron::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_id_neuron;
    pStream << m_accumulated;
    pStream << m_threshold;
    pStream << m_activation_function;
    pStream << m_is_bias;
    pStream << static_cast<qint32>(m_next.size());
    for (const auto & next : m_next)
    {
        pStream << next->neuron()->getNeuronId();
        next->serializeObject(pStream);
    }
}

void Neuron::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_id_neuron;
    pStream >> m_accumulated;
    pStream >> m_threshold;
    pStream >> m_activation_function;
    pStream >> m_is_bias;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        qint32 id = 0;
        pStream >> id;
        Neuron* previous = nullptr;
        if (m_layer->getPreviousLayer() != nullptr)
        {
            previous = m_layer->getPreviousLayer()->getNeuron(id);
        }
        spEdge pEdge = spEdge::create(previous, this, 0);
        pEdge->deserializeObject(pStream);
        m_next.append(pEdge);
        if (previous != nullptr)
        {
            previous->addPrevious(pEdge);
        }
    }
}
