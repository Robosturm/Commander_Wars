#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QMap>
#include <QVector>
#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "ai/neuralnetwork/neural/layer.h"
#include "ai/neuralnetwork/neural/neuron.h"
#include "ai/neuralnetwork//dataset/dataset.h"

class NeuralNetwork;
class Neuron;
class Layer;
using spNeuralNetwork = oxygine::intrusive_ptr<NeuralNetwork>;

class NeuralNetwork : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:

    NeuralNetwork(double maxWeight = 5);
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

    QString outputString();

    QVector<QVector<QVector<double>>> getWeights();
    QVector<QVector<QVector<spEdge>>> getEdges();
    QVector<spLayer> getLayers();

    /**
     * @brief toString for debugging
     * @return
     */
    QString toString();
    /**
     * @brief shiftBackWeights for learning
     * @param weights
     */
    void alterWeights(const QVector<QVector<QVector<double>>>& weights);
    /**
     * @brief shiftBackWeights for learning
     * @param weights
     */
    void shiftBackWeights(const QVector<QVector<QVector<double>>>& weights);
    /**
     * @brief loss for learning
     * @param in
     * @param out
     * @return
     */
    double loss(const QVector<double>& in, const QVector<double>& out);
    /**
     * @brief loss for learning
     * @param ins
     * @param outs
     * @return
     */
    double loss(const QVector<QVector<double>>& ins, const QVector<QVector<double>>& outs);
    /**
     * @brief shiftWeights for learning
     * @param percentage_of_range
     */
    void shiftWeights(float percentage_of_range);
    /**
     * @brief predictAllForScore for learning
     * @param in
     * @return
     */
    double predictAllForScore(const spDataset dataset, Dataset::Datatype d = Dataset::Datatype::TEST, qint32 limit=-1);
    /**
     * @brief getLearningRate  for training
     * @return
     */
    double getLearningRate() const;
    /**
     * @brief setLearningRate for training
     * @param learningRate
     */
    void setLearningRate(double learningRate);
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
    double m_learningRate = 1;
    QString m_networkName;
};

#endif // NEURALNETWORK_H
