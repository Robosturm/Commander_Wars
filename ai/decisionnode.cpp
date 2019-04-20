#include "decisionnode.h"

#include "leaf.h"

DecisionNode::DecisionNode(spDecisionQuestion pQuestion, QVector<spDecisionNode> pNodes)
	: m_pQuestion(pQuestion),
      m_pNodes(pNodes)
{
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

void DecisionNode::serializeObject(QDataStream& pStream)
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
    m_pQuestion = new DecisionQuestion;
    m_pQuestion->deserializeObject(pStream);
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        bool isNode = false;
        pStream >> isNode;
        if (isNode)
        {
            m_pNodes.append(new DecisionNode());
            m_pNodes[i]->deserializeObject(pStream);
        }
        else
        {
            m_pNodes.append(new Leaf());
            m_pNodes[i]->deserializeObject(pStream);
        }
    }
}
