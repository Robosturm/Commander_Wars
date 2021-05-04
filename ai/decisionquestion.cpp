#include "ai/decisionquestion.h"

DecisionQuestion::DecisionQuestion()
{
    setObjectName("DecisionQuestion");
}

void DecisionQuestion::appendQuestion(spQuestion pQuestions)
{
    m_Questions.append(pQuestions);
}

void DecisionQuestion::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_Questions.size());
    for (qint32 i = 0; i < m_Questions.size(); i++)
    {
        m_Questions[i]->serializeObject(pStream);
    }
}

void DecisionQuestion::deserializeObject(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    m_Questions.clear();
    for (qint32 i = 0; i < size; i++)
    {
        m_Questions.append(spQuestion::create());
        m_Questions[i]->deserializeObject(pStream);
    }
}

QString DecisionQuestion::print()
{
    QString ret = "";
    for (qint32 i = 0; i < m_Questions.size(); i++)
    {
        ret += m_Questions[i]->print() + " ";
    }
    return ret;
}

qint32 DecisionQuestion::matches(float value)
{
    for (qint32 i = 0; i < m_Questions.size(); i++)
    {
        if (m_Questions[i]->matches(value))
        {
            return i;
        }
    }
    return -1;
}

qint32 DecisionQuestion::matches(QVector<float> &rData)
{
    for (qint32 i = 0; i < m_Questions.size(); i++)
    {
        if (m_Questions[i]->matches(rData))
        {
            return i;
        }
    }
    return -1;
}
