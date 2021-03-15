#ifndef EDGE_H
#define EDGE_H

#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "coreengine/fileserializable.h"

class NeuralNetwork;
class Neuron;
class Layer;
class Edge;

using spNeuron = oxygine::intrusive_ptr<Neuron>;
using spEdge = oxygine::intrusive_ptr<Edge>;

//Edge between two neurons
class Edge : public oxygine::ref_counter, public FileSerializable
{
public:
    Edge(Neuron* n, Neuron* start, double w );
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

    Neuron* neuron() const;

    Neuron* neuronb() const;

    double weight();

    double weightP();

    void propagate(double neuron_output);

    void alterWeight(double w);

    void shiftWeight(double dw);

    double getLastShift() const;

    void resetLastShift();

    double backpropagationMemory() const;

    void setBackpropagationMemory(double v);

private:
    Neuron* m_n = nullptr;
    Neuron* m_nb = nullptr;
    double m_w = 0.0;
    double m_last_shift = 0;
    double m_backpropagation_memory{0};

};

#endif // EDGE_H
