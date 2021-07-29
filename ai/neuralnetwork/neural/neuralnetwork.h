#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QMap>
#include <QVector>
#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "ai/neuralnetwork/neural/layer.h"
#include "ai/neuralnetwork/neural/neuron.h"

class NeuralNetwork;
class Neuron;
class Layer;
using spNeuralNetwork = oxygine::intrusive_ptr<NeuralNetwork>;

class NeuralNetwork : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:

    NeuralNetwork(double maxWeight = 1.0);
    virtual ~NeuralNetwork()= default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 1;
    }
    /**
     * @brief addLayer adds a layer to the neural network
     * @param parameters
     * TYPE (Standard=0, OUTPUT=1, INPUT=2)
     * SIZE
     * ACTIVATION (LINEAR=0, SIGMOID=1, RELU=2)
     */
    void addLayer(QMap<QString, double> parameters);
    /**
     * @brief autogenerate connects all neurons and can be used to randomize all weights
     * @param randomize
     */
    void autogenerate(bool randomize = true);
    /**
     * @brief predicts the score for the input calls
     * @param in
     * @return
     */
    QVector<double> predict(const QVector<double>& in);
    /**
     * @brief output
     * @return the last predicted output
     */
    QVector<double> output();
    /**
     * @brief toString for debugging
     * @return
     */
    QString toString();
    /**
     * @brief getNetworkName
     * @return
     */
    const QString &getNetworkName() const;
    /**
     * @brief setNetworkName
     * @param newNetworkName
     */
    void setNetworkName(const QString &newNetworkName);
    /**
     * @brief mutateNeuralNetwork
     * @param mutationChance
     */
    void mutateAllWeights(double mutationChance);
private:
    void setInput(QVector<double> in);
    void trigger();
    void clean();
    void connectComplete();
    void randomizeAllWeights();
private:
    QVector<spLayer> m_layers;
    double m_fitness;
    QVector<QMap<QString, double>> m_configuration;
    double m_maxWeight = 1;
    QString m_networkName;
};

#endif // NEURALNETWORK_H
