#ifndef EDGE_H
#define EDGE_H

#include <QVector>

#include "coreengine/fileserializable.h"

class NeuralNetwork;
class Neuron;
class Layer;
class Edge;

using spNeuron = std::shared_ptr<Neuron>;
using spEdge = std::shared_ptr<Edge>;

//Edge between two neurons
class Edge final : public FileSerializable
{
public:
    Edge(Neuron* nextNeuron, Neuron* previousNeuron, double weight);
    ~Edge() = default;
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
    Neuron* nextNeuron() const;
    Neuron* previousNeuron() const;
    double weight();
    void propagate(double neuron_output);
    void alterWeight(double w);
private:
    Neuron* m_nextNeuron = nullptr;
    Neuron* m_previousNeuron = nullptr;
    double m_weight = 0.0;
};

#endif // EDGE_H
