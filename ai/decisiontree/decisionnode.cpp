#include "ai/decisiontree/decisionnode.h"
#include "ai/decisiontree/leaf.h"
#include "coreengine/interpreter.h"

DecisionNode::DecisionNode(spDecisionQuestion & pQuestion, const std::vector<spDecisionNode> & pNodes)
	: m_pQuestion(pQuestion),
      m_pNodes(pNodes)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DecisionNode");
#endif
    Interpreter::setCppOwnerShip(this);
}

DecisionNode::DecisionNode()
{
#ifdef GRAPHICSUPPORT
    setObjectName("DecisionNode");
#endif
    Interpreter::setCppOwnerShip(this);
}

float DecisionNode::getDecision(std::vector<float>& input)
{
    qint32 result = m_pQuestion->matches(input);
    if (result >= 0 && result < m_pNodes.size())
    {
        return m_pNodes[result]->getDecision(input);
    }
    return -1.0f;
}

void DecisionNode::serializeObject(QDataStream& pStream) const
{
    pStream << true; // --> 0 for node and 1 for leaf
    pStream << getVersion();
    m_pQuestion->serializeObject(pStream);
    pStream << static_cast<qint32>(m_pNodes.size());
    for (auto & node : m_pNodes)
    {
        node->serializeObject(pStream);
    }
}

void DecisionNode::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    m_pQuestion = MemoryManagement::create<DecisionQuestion>();
    m_pQuestion->deserializeObject(pStream);
    qint32 size = 0;
    pStream >> size;
    m_pNodes.clear();
    for (qint32 i = 0; i < size; i++)
    {
        bool isNode = false;
        pStream >> isNode;
        if (isNode)
        {
            m_pNodes.push_back(MemoryManagement::create<DecisionNode>());
            m_pNodes[i]->deserializeObject(pStream);
        }
        else
        {
            m_pNodes.push_back(MemoryManagement::create<Leaf>());
            m_pNodes[i]->deserializeObject(pStream);
        }
    }
}
