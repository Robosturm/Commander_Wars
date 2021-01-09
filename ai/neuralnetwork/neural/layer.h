#ifndef LAYER_H
#define LAYER_H

#include <QMap>
#include <QVector>
#include "oxygine-framework.h"

#include "ai/neuralnetwork/neural/neuron.h"

class NeuralNetwork;
class Layer;

using spLayer = oxygine::intrusive_ptr<Layer>;
//Layer of the network
class Layer : public oxygine::ref_counter
{
public:
    static const QString LAYER_PARAMETER_SIZE;
    static const QString LAYER_PARAMETER_TYPE;
    static const QString LAYER_PARAMETER_ACTIVATION;
    enum class LayerType
    {
        STANDARD = 0, //Standard layer : fully connected perceptrons
        OUTPUT, // Output : No bias neuron
        INPUT, // Input: Standard input (output of neurons is outputRaw() )
        SOFTMAX //K-Class Classification Layer

    };
    Layer(qint32 id_layer, NeuralNetwork* net, QMap<QString, double> parameters);

    ~Layer();

    qint32 getId() const;

    void initLayer();

    void clean();

    void trigger();

    void connectComplete(Layer* next);

    QVector<double> output();

    QVector<spNeuron> neurons() const;

    void alterWeights(const QVector<QVector<double> >& weights);

    void shiftBackWeights(const QVector<QVector<double> >& weights);

    QVector<QVector<double>> getWeights();

    QVector<QVector<spEdge>> getEdges();

    void randomizeAllWeights(double abs_value);

    QString toString();

    void shiftWeights(float range);

    const QMap<QString, double>& getParameters() const;

    QVector<QVector<double>> getBackpropagationShifts(const QVector<double>& target);

    LayerType getType() const;

    Neuron::ActivationFunction getActivation() const;

    NeuralNetwork* getNet() const { return m_net; }

private:
    NeuralNetwork* m_net;
    qint32 m_id_layer;
    QVector<spNeuron> m_neurons;
    LayerType m_type;
    Neuron::ActivationFunction m_activation;
    QMap<QString, double> m_parameters;
};

#endif // LAYER_H
