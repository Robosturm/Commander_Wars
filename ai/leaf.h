#ifndef AI_LEAF
#define AI_LEAF

#include "decisionnode.h"

#include "QObject"
#include "qstring.h"

#include "coreengine/fileserializable.h"

class Leaf : public DecisionNode
{
    Q_OBJECT
public:
        Leaf() = default;
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
        qint32 totalChance{0};
};

#endif
