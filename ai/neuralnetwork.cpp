#include "neuralnetwork.h"

#include <qcryptographichash.h>
#include <qfile.h>
#include <qdatastream.h>
#include "ai/decisionquestion.h"
#include "ai/coreai.h"

NeuralNetwork::NeuralNetwork(quint32 inputs, quint32 layerNumber, quint32 output)
    : QObject(),
      OpenNN::NeuralNetwork (inputs, layerNumber, output)
{
}

void NeuralNetwork::loadTrainingData(QString networkFile, QString trainingDataFile, qint32 outputSize)
{
    QFile file(networkFile);
    QFile trainingFile(trainingDataFile);

    trainingFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QCryptographicHash myHash(QCryptographicHash::Sha3_512);
    myHash.addData(&trainingFile);
    QByteArray hash = myHash.result();
    trainingFile.close();

    bool needsTraining = true;
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        QByteArray currentHash;
        for (qint32 i = 0; i < hash.size(); i++)
        {
            qint8 value = 0;
            stream >> value;
            currentHash.append(value);
        }
        // check if the training data has changed
        if (currentHash == hash)
        {
            deserializeObject(stream);
            needsTraining = false;
        }
        file.close();
    }
    if (needsTraining)
    {
        QVector<QVector<float>> trainingData;
        QVector<QVector<spDecisionQuestion>> questions;
        CoreAI::getTrainingData(trainingDataFile, trainingData, questions);
        train(trainingData, outputSize);
        // store trained tree for next use.
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        for (qint32 i = 0; i < hash.size(); i++)
        {
            stream << static_cast<qint8>(hash[i]);
        }
        serializeObject(stream);
    }
}

void NeuralNetwork::train(QVector<QVector<float>>& trainingData, qint32 outputSize)
{
    if (trainingData.size() > 0)
    {

        OpenNN::Matrix<double> matrix(trainingData.size(), trainingData[0].size());
        for (qint32 i = 0; i < trainingData.size(); i++)
        {
            for (qint32 i2 = 0; i2 < trainingData[i].size(); i2++)
            {
                matrix[i * trainingData[i].size() + i2] = trainingData[i][i2];
            }
        }
        OpenNN::DataSet dataSet(matrix);
        OpenNN::Variables* variables_pointer = dataSet.get_variables_pointer();
        variables_pointer->set(trainingData[0].size() - outputSize, outputSize);
        OpenNN::TrainingStrategy training_strategy(this, &dataSet);
        OpenNN::QuasiNewtonMethod* quasi_Newton_method_pointer = training_strategy.get_quasi_Newton_method_pointer();
        quasi_Newton_method_pointer->set_maximum_epochs_number(10000);
        quasi_Newton_method_pointer->set_minimum_loss_decrease(1.0e-6);
        training_strategy.set_display(false);
        training_strategy.perform_training();
    }
}

QVector<double> NeuralNetwork::calculateOutput(QVector<double> input)
{
    OpenNN::Vector<double> inputs(input.size());
    for (qint32 i = 0; i < input.size(); i++)
    {
        inputs[i] = input[i];
    }
    OpenNN::Vector<double> outputs = OpenNN::NeuralNetwork::get_multilayer_perceptron_pointer()->calculate_outputs(inputs.to_row_matrix());
    QVector<double> output(outputs.size());
    for (qint32 i = 0; i < outputs.size(); i++)
    {
        output[i] = outputs[i];
    }
    return output;
}

void NeuralNetwork::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    OpenNN::Vector<double> parameters = OpenNN::NeuralNetwork::get_parameters();
    pStream << static_cast<quint32>(parameters.size());
    for (quint32 i = 0; i < parameters.size(); i++)
    {
        pStream << parameters[i];
    }
}

void NeuralNetwork::deserializeObject(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    OpenNN::Vector<double> parameters;
    quint32 size = 0;
    pStream >> size;
    for (quint32 i = 0; i < size; i++)
    {
        double value = 0;
        pStream >> value;
        parameters.push_back(value);
    }
    OpenNN::NeuralNetwork::set_parameters(parameters);
}

