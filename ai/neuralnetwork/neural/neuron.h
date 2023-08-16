#ifndef NEURON_H
#define NEURON_H

#include <vector>

#include "ai/neuralnetwork/neural/edge.h"

#include "coreengine/fileserializable.h"

class Layer;

class Neuron;
using spNeuron = std::shared_ptr<Neuron>;

class Neuron final : public FileSerializable
{
public:

    enum class ActivationFunction
    {
        LINEAR,
        SIGMOID,
        RELU,
        Step,
        Limited,
        Bias,
    };

    Neuron(qint32 id_neuron, Layer* layer, ActivationFunction function = ActivationFunction::LINEAR, bool is_bias = false);
   virtual ~Neuron() = default;

    void trigger();
    double output();
    void clean();
    void addAccumulated(double v);
    void addNext(spNeuron & n, bool random = true);
    void addNext(spEdge & e);
    void addPrevious(spEdge & e);
    qint32 getNeuronId() const;

    void setAccumulated(double v);

    void randomizeAllWeights(double abs_value);
    void mutateAllWeights(double mutationChance, double maxWeight, double mutationRate = 0.1f);

    QString toString();

    bool isBias() const;
    const Layer* getLayer() const
    {
        return m_layer;
    }
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
private:

private:
    Layer* m_layer = nullptr;
    qint32 m_id_neuron = 0;
    double m_accumulated = 0.0;

    double m_threshold = 0.0;
    std::vector<spEdge> m_next;
    std::vector<spEdge> m_previous;
    ActivationFunction m_activation_function;
    bool m_is_bias = false;
};

#endif // NEURON_H
