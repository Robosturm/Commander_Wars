#include "ai/decisionnode.h"

#include "ai/leaf.h"

DecisionNode::DecisionNode(spDecisionQuestion pQuestion, QVector<spDecisionNode> pNodes)
	: m_pQuestion(pQuestion),
      m_pNodes(pNodes)
{
    setObjectName("DecisionNode");
}

DecisionNode::DecisionNode()
{
    setObjectName("DecisionNode");
}

DecisionNode::~DecisionNode()
{
}

float DecisionNode::getDecision(QVector<float>& input)
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
    for (qint32 i = 0; i < m_pNodes.size(); i++)
    {
        m_pNodes[i]->serializeObject(pStream);
    }
}

void DecisionNode::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    m_pQuestion = spDecisionQuestion::create();
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
            m_pNodes.append(spDecisionNode::create());
            m_pNodes[i]->deserializeObject(pStream);
        }
        else
        {
            m_pNodes.append(spLeaf::create());
            m_pNodes[i]->deserializeObject(pStream);
        }
    }
}
