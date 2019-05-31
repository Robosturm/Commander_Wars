#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QObject>

#include "opennn/opennn.h"
#include "oxygine-framework.h"

#include "coreengine/fileserializable.h"

class NeuralNetwork;
typedef oxygine::intrusive_ptr<NeuralNetwork> spNeuralNetwork;

class NeuralNetwork : public QObject, public OpenNN::NeuralNetwork, public oxygine::ref_counter, public FileSerializable
{
    Q_OBJECT
public:
    explicit NeuralNetwork(quint32 inputs, quint32 layerNumber, quint32 output);
    virtual void serializeObject(QDataStream& pStream)  override;
    virtual void deserializeObject(QDataStream& pStream)  override;
    virtual qint32 getVersion() override
    {
        return 1;
    }
    /**
     * @brief loadTrainingData
     * @param treeFile
     * @param trainingDataFile
     */
    void loadTrainingData(QString networkFile, QString trainingDataFile, qint32 outputSize);
    /**
     * @brief loadTrain
     * @param trainingData
     */
    void train(QVector<QVector<float>>& trainingData, qint32 outputSize);

    QVector<double> calculateOutput(QVector<double> input);
signals:

public slots:
};

#endif // NEURALNETWORK_H
