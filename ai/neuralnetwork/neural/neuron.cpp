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
    if(m_layer->getType() == Layer::LayerType::INPUT)
    {
        m_activation_function = ActivationFunction::LINEAR;
    }
    else if (m_layer->getType() == Layer::LayerType::OUTPUT)
    {
        m_activation_function = ActivationFunction::LINEAR;
    }
    else if (m_is_bias)
    {
        m_activation_function = ActivationFunction::Bias;
    }
}

void Neuron::trigger()
{
    double value = output();
    for (spEdge & e : m_next)
    {
        e->propagate(value);
    }

}

double Neuron::output()
{
    double ret = 0;
    switch (m_activation_function)
    {
        case ActivationFunction::Bias:
        {
            ret = 1;
            break;
        }
        case ActivationFunction::Limited:
        {
            if (m_accumulated > 1)
            {
                ret = 1;
            }
            else if (m_accumulated < -1)
            {
                ret = -1;
            }
            else
            {
                ret = m_accumulated;
            }
            break;
        }
        case ActivationFunction::LINEAR:
        {
            ret = m_accumulated;
            break;
        }
        case ActivationFunction::RELU:
        {
            ret = GlobalUtils::relu(m_accumulated);
            break;
        }
        case ActivationFunction::SIGMOID:
        {
            ret = GlobalUtils::sigmoid(m_accumulated);
            break;
        }
        case ActivationFunction::Step:
        {
            if (m_accumulated > 0)
            {
                ret = 1;
            }
            else
            {
                ret = -1;
            }
            break;
        }
        default:
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "Neuron::output invalid output function defined");
        }
    }
    return ret;
}

void Neuron::clean()
{
    m_accumulated = 0;
}

void Neuron::addAccumulated(double v)
{
    m_accumulated += v;
}

void Neuron::addNext(spNeuron n)
{
    m_next.push_back(spEdge::create(n.get(), this, GlobalUtils::randDouble(-1, 1)));
    n->addPrevious(m_next.back());
}

void Neuron::addNext(spEdge e)
{
   m_next.push_back(e);
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

void Neuron::randomizeAllWeights(double abs_value)
{
    for(spEdge & e : m_next)
    {
        e->alterWeight(GlobalUtils::randDouble(-abs_value, abs_value));
    }
}

void Neuron::mutateAllWeights(double mutationChance, double maxWeight)
{
    for(spEdge & e : m_next)
    {
        if (GlobalUtils::randDouble(0, 1) < mutationChance)
        {
            e->alterWeight(GlobalUtils::randDouble(-maxWeight, maxWeight));
        }
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
    // store the connection to the previous layer
    pStream << static_cast<qint32>(m_previous.size());
    for (const auto & previous : m_previous)
    {
        pStream << previous->previousNeuron()->getNeuronId();
        previous->serializeObject(pStream);
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
        // read the connection to the previous layer
        qint32 id = 0;
        pStream >> id;
        Neuron* previous = nullptr;
        if (m_layer->getPreviousLayer() != nullptr)
        {
            previous = m_layer->getPreviousLayer()->getNeuron(id);
        }
        spEdge pEdge = spEdge::create(this, previous, 0);
        pEdge->deserializeObject(pStream);
        m_previous.append(pEdge);
        if (previous != nullptr)
        {
            previous->addNext(pEdge);
        }
    }
}
