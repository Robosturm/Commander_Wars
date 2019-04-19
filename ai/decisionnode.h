#ifndef AI_DECISIONNODE
#define AI_DECISIONNODE

#include "QObject"

#include "oxygine-framework.h"

#include "question.h"

#include "coreengine/fileserializable.h"

class DecisionNode;
typedef oxygine::intrusive_ptr<DecisionNode> spDecisionNode;

class DecisionNode : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
        DecisionNode() = default;
        DecisionNode(spQuestion pQuestion, spDecisionNode pNodeTrue, spDecisionNode pNodeFalse);
        virtual ~DecisionNode();

        virtual void serializeObject(QDataStream& pStream)  override;
        virtual void deserializeObject(QDataStream& pStream)  override;
        virtual qint32 getVersion() override
        {
            return 1;
        }
        inline DecisionNode* getNodeTrue()
        {
            return m_pNodeTrue.get();
        }
        inline DecisionNode* getNodeFalse()
        {
            return m_pNodeFalse.get();
        }
        inline Question* getQuestion()
        {
            return m_pQuestion.get();
        }
public slots:
        virtual float getDecision(QVector<float>& input);
private:
        spQuestion m_pQuestion;
        spDecisionNode m_pNodeTrue;
        spDecisionNode m_pNodeFalse;
};

#endif
