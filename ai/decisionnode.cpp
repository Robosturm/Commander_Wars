#include "decisionnode.h"

#include "leaf.h"

DecisionNode::DecisionNode(spQuestion pQuestion, spDecisionNode pNodeTrue, spDecisionNode pNodeFalse)
	: m_pQuestion(pQuestion),
	  m_pNodeTrue(pNodeTrue),
	  m_pNodeFalse(pNodeFalse)
{
}

DecisionNode::~DecisionNode()
{
}

float DecisionNode::getDecision(QVector<float>& input)
{
	if (m_pQuestion->matches(input))
	{
		return m_pNodeTrue->getDecision(input);
	}
	else
	{
		return m_pNodeFalse->getDecision(input);
	}
}

void DecisionNode::serialize(QDataStream& pStream)
{
    pStream << true; // --> 0 for node and 1 for leaf
    pStream << getVersion();
    m_pQuestion->serialize(pStream);
    m_pNodeTrue->serialize(pStream);
    m_pNodeFalse->serialize(pStream);
}

void DecisionNode::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    m_pQuestion = new Question(0, 0);
    m_pQuestion->deserialize(pStream);
    bool isNode = false;
    pStream >> isNode;
    if (isNode)
    {
        m_pNodeTrue = new DecisionNode();
        m_pNodeTrue->deserialize(pStream);
    }
    else
    {
        m_pNodeTrue = new Leaf();
        m_pNodeTrue->deserialize(pStream);
    }
    pStream >> isNode;
    if (isNode)
    {
        m_pNodeFalse = new DecisionNode();
        m_pNodeFalse->deserialize(pStream);
    }
    else
    {
        m_pNodeFalse = new Leaf();
        m_pNodeFalse->deserialize(pStream);
    }
}
