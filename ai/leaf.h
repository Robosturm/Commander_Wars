#ifndef AI_LEAF
#define AI_LEAF

#include <QObject>
#include <QString>

#include "ai/decisionnode.h"
#include "coreengine/fileserializable.h"

class Leaf;
using spLeaf = oxygine::intrusive_ptr<Leaf>;

class Leaf : public DecisionNode
{
    Q_OBJECT
public:
        explicit Leaf();
        Leaf(QVector<QVector<float>>& trainingData);
        virtual ~Leaf() = default;
        virtual void serializeObject(QDataStream& pStream) const override;
        virtual void deserializeObject(QDataStream& pStream)  override;
        virtual qint32 getVersion() const override
        {
            return 1;
        }
        QString print();
public slots:
        virtual float getDecision(QVector<float>& input) override;
private:
        QVector<qint32> m_AnswersChances;
        QVector<float> m_Answers;
        qint32 m_totalChance{0};
};

#endif
