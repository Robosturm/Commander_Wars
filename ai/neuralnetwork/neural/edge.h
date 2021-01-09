#ifndef EDGE_H
#define EDGE_H

#include <QVector>
#include "oxygine-framework.h"

class NeuralNetwork;
class Neuron;
class Layer;
class Edge;

using spNeuron = oxygine::intrusive_ptr<Neuron>;
using spEdge = oxygine::intrusive_ptr<Edge>;

//Edge between two neurons
class Edge : public oxygine::ref_counter
{
public:
    Edge(Neuron* n, Neuron* start, double w );

    Neuron* neuron() const;

    Neuron* neuronb() const;

    double weight();

    double* weightP();

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
    double m_backpropagation_memory;

};

#endif // EDGE_H
