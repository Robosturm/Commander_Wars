#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QMap>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "ai/neuralnetwork/neural/layer.h"
#include "ai/neuralnetwork/neural/neuron.h"
#include "ai/neuralnetwork//dataset/dataset.h"

class NeuralNetwork;
class Neuron;
class Layer;
using spNeuralNetwork = oxygine::intrusive_ptr<NeuralNetwork>;

class NeuralNetwork : public oxygine::ref_counter, public FileSerializable
{
public:

    NeuralNetwork(double maxWeight = 1);

    ~NeuralNetwork();
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

    void autogenerate(bool randomize = true);

    void addLayer(QMap<QString, double> parameters);

    void clean();

    void setInput(QVector<double> in);

    void trigger();

    QVector<double> output();

    QString outputString();

    void connectComplete();

    void alterWeights(const QVector<QVector<QVector<double>>>& weights);

    void shiftBackWeights(const QVector<QVector<QVector<double>>>& weights);

    QVector<QVector<QVector<double>>> getWeights();

    QVector<QVector<QVector<spEdge>>> getEdges();

    void randomizeAllWeights();

    double loss(const QVector<double>& in, const QVector<double>& out);

    double loss(const QVector<QVector<double>>& ins, const QVector<QVector<double>>& outs);

    QString toString();

    void shiftWeights(float percentage_of_range);

    QVector<double> predict(const QVector<double>& in);

    double predictAllForScore(const spDataset dataset, Dataset::Datatype d = Dataset::Datatype::TEST, qint32 limit=-1);

    QVector<spLayer> getLayers();

    double getLearningRate() const;
    void setLearningRate(double learningRate);

private:
    QVector<spLayer> m_layers;
    double m_fitness;
    QVector<QMap<QString, double>> m_configuration;
    double m_maxWeight = 1;
    double m_learningRate = 1;
};

#endif // NEURALNETWORK_H
