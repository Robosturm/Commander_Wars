#pragma once

#include "ai/neuralnetwork/dataset/dataset.h"
#include "ai/neuralnetwork/neural/neuralnetwork.h"

class Optimizer
{
public:
        Optimizer(double learningRate = 1);
	~Optimizer();

	virtual void minimize();

        void setNeuralNetwork(spNeuralNetwork net);

        void setDataset(spDataset dataset);

        double getScore(Dataset::Datatype d, qint32 limit = -1);

protected:
        spNeuralNetwork m_n;
        spDataset m_d;
        double m_learningRate = 1.0;
};

