#include "ai/neuralnetwork/neural/neuralnetwork.h"
#include "ai/neuralnetwork/neural/neuron.h"
#include "ai/neuralnetwork/neural/layer.h"

#include "coreengine/globalutils.h"

NeuralNetwork::NeuralNetwork(double maxWeight)
    : m_maxWeight(maxWeight)
{
}

NeuralNetwork::~NeuralNetwork()
{
}


void NeuralNetwork::autogenerate(bool randomize)
{
	connectComplete();
	if(randomize)
    {
		randomizeAllWeights();
    }
	
}

void NeuralNetwork::addLayer(QMap<QString, double> parameters)
{
    m_configuration.append(parameters);
    m_layers.push_back(spLayer::create(m_layers.size(), this, parameters));
}


void NeuralNetwork::clean()
{
    for(spLayer & l : m_layers)
    {
        l->clean();
    }
}

void NeuralNetwork::setInput(QVector<double> in)
{
	clean();
    for(qint32 i = 0; i < in.size(); ++i)
    {
        m_layers[0]->neurons()[i]->setAccumulated(in[i]);
    }
}

void NeuralNetwork::trigger()
{
    for (spLayer & l : m_layers)
    {
		l->trigger();
    }
}

QVector<double> NeuralNetwork::output()
{
    return (m_layers.back())->output();
}

QString NeuralNetwork::outputString()
{
    QString s;
    for (qint32 i = 0; i < (m_layers.back())->output().size(); i++)
	{
        double out = (m_layers.back())->output()[i];
        s += QString::number(out) + " ";
	}
	return s;
}

void NeuralNetwork::connectComplete()
{
    for(qint32 i_layer = 0; i_layer < m_layers.size() - 1; ++i_layer)
    {
        m_layers[i_layer]->connectComplete(m_layers[i_layer + 1].get());
    }
}

void NeuralNetwork::alterWeights(const QVector<QVector<QVector<double> > >& weights)
{
    for (qint32 i_layer = 0; i_layer < m_layers.size() - 1; ++i_layer)
    {
        m_layers[i_layer]->alterWeights(weights[i_layer]);
    }
}

void NeuralNetwork::shiftBackWeights(const QVector<QVector<QVector<double> > >& weights)
{
    for (qint32 i_layer = m_layers.size() - 1; i_layer >= 0; --i_layer)
    {
		if(weights[i_layer].size() != 0)
        {
            m_layers[i_layer]->shiftBackWeights(weights[i_layer]);
        }
    }
}

QVector<QVector<QVector<double>>> NeuralNetwork::getWeights()
{
    QVector<QVector<QVector<double>>> w;
    w.reserve(m_layers.size() - 1);
    for (qint32 i_layer = 0; i_layer < m_layers.size() - 1; ++i_layer)
    {
        w.push_back(m_layers[i_layer]->getWeights());
    }
    return w;
}

QVector<QVector<QVector<spEdge>>> NeuralNetwork::getEdges()
{
    QVector<QVector<QVector<spEdge>>> w;
    w.reserve(m_layers.size() - 1);
    for (qint32 i_layer = 0; i_layer < m_layers.size() - 1; ++i_layer)
    {
        w.push_back(m_layers[i_layer]->getEdges());
    }
    return w;
}

void NeuralNetwork::randomizeAllWeights()
{
    for(qint32 i_layer = 0; i_layer < m_layers.size() - 1; ++i_layer)
    {
        m_layers[i_layer]->randomizeAllWeights(m_maxWeight);
    }
}

double NeuralNetwork::loss(const QVector<double>& in, const QVector<double>& out)
{
	double sum = 0;
	auto out_exp = predict(in);
    auto activation = static_cast<Neuron::ActivationFunction>(m_layers.back()->getParameters()[Layer::LAYER_PARAMETER_ACTIVATION]);
    if (activation == Neuron::ActivationFunction::SIGMOID)
    {
        for (qint32 i = 0; i < out.size(); ++i)
        {
			sum += 0.5 * (out[i] - out_exp[i]) * (out[i] - out_exp[i]);
        }
    }
    else if (activation == Neuron::ActivationFunction::LINEAR)
    {
        for (qint32 i = 0; i < out.size(); ++i)
        {
			sum += (out[i] - out_exp[i]) * (out[i] - out_exp[i]);
        }
    }
	return sum;
}

double NeuralNetwork::loss(const QVector<QVector<double>>& ins, const QVector<QVector<double>>& outs)
{
	double sum = 0;
    for (qint32 i = 0; i < ins.size(); i++)
	{
        sum += loss(ins[i], outs[i]);
	}
	return sum / ins.size() ;
}

QString NeuralNetwork::toString()
{
    QString s = "NeuralNetwork";
    s.push_back('\n');
    for(spLayer & l : m_layers)
    {
        s += l->toString();
    }
    return s;
}


void NeuralNetwork::shiftWeights(float percentage_of_range)
{
    float range = percentage_of_range * (m_maxWeight + m_maxWeight);
    for(spLayer & l : m_layers)
    {
		l->shiftWeights(range);
    }
}


QVector<double> NeuralNetwork::predict(const QVector<double>& in)
{
	setInput(in);
	trigger();
	return output();
}

double NeuralNetwork::predictAllForScore(const spDataset dataset, Dataset::Datatype d,  qint32 limit)
{
	if (limit == 0)
    {
		return 1;
    }
	double s = 0;

	if (limit == -1)
    {
        for (qint32 i = 0; i < dataset->getIns(d).size(); i++)
        {
            s += GlobalUtils::distanceVector(predict(dataset->getIns(d)[i]), dataset->getOuts(d)[i]);
        }
    }
	else
    {
        for (qint32 i = 0; i < limit; i++)
		{
            qint32 r = GlobalUtils::randInt(0, dataset->getIns(d).size());
            s += GlobalUtils::distanceVector(predict(dataset->getIns(d)[r]), dataset->getOuts(d)[r]);
		}
    }
	if (limit == -1)
    {
        s /= dataset->getIns(d).size();
    }
	else
    {
		s /= limit;
    }
	return s;
}

QVector<spLayer> NeuralNetwork::getLayers()
{
    return m_layers;
}

double NeuralNetwork::getLearningRate() const
{
    return m_learningRate;
}

void NeuralNetwork::setLearningRate(double learningRate)
{
    m_learningRate = learningRate;
}

void NeuralNetwork::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_fitness;
    pStream << m_configuration;
    pStream << m_maxWeight;
    pStream << m_learningRate;
    pStream << static_cast<qint32>(m_layers.size());
    for (const auto & layer : m_layers)
    {
        layer->serializeObject(pStream);
    }
}

void NeuralNetwork::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_fitness;
    pStream >> m_configuration;
    pStream >> m_maxWeight;
    pStream >> m_learningRate;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        spLayer layer = spLayer::create(i, this, m_configuration[i]);
        if (i > 0)
        {
            layer->setPreviousLayer(m_layers[i - 1].get());
        }
        layer->deserializeObject(pStream);
        m_layers.append(layer);
    }
}
