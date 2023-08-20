#pragma once
#include <QObject>
#include <QString>

#include "ai/decisiontree/decisionnode.h"

class Leaf;
using spLeaf = std::shared_ptr<Leaf>;

class Leaf final : public DecisionNode
{
    Q_OBJECT
public:
        explicit Leaf();
        Leaf(std::vector<std::vector<float>>& trainingData);
        ~Leaf() = default;
        virtual void serializeObject(QDataStream& pStream) const override;
        virtual void deserializeObject(QDataStream& pStream)  override;
        virtual qint32 getVersion() const override
        {
            return 1;
        }
        QString print();
public slots:
        virtual float getDecision(std::vector<float>& input) override;
private:
        std::vector<qint32> m_AnswersChances;
        std::vector<float> m_Answers;
        qint32 m_totalChance{0};
};
