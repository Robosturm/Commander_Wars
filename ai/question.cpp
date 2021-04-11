#include "ai/question.h"

Question::Question()
{
    setObjectName("Question");
}

Question::Question(float value, qint32 index, GameEnums::AIQuestionType QuestionType)
	: m_MinValue(value),
	  m_MaxValue(value),
      m_Index(index),
      m_QuestionType(QuestionType)
{
    setObjectName("Question");
}

Question::Question(float minValue, float maxValue, qint32 index, GameEnums::AIQuestionType QuestionType)
	: m_MinValue(minValue),
	  m_MaxValue(maxValue),
      m_Index(index),
      m_QuestionType(QuestionType)
{
    setObjectName("Question");
}

bool Question::matches(float value)
{
    switch (m_QuestionType)
    {
        case GameEnums::AIQuestionType_Between:
        {
            if ((value >= m_MinValue) &&
                (value < m_MaxValue))
            {
                return true;
            }
            break;
        }
        case GameEnums::AIQuestionType_Equal:
        {
            if (value == m_MinValue)
            {
                return true;
            }
            break;
        }
        case GameEnums::AIQuestionType_Greater:
        {
            if (value >= m_MaxValue)
            {
                return true;
            }
            break;
        }
        case GameEnums::AIQuestionType_Smaler:
        {
            if (value < m_MinValue)
            {
                return true;
            }
            break;
        }
    }
	return false;
}

bool Question::matches(QVector<float> &rData)
{
	return matches(rData[m_Index]);
}

qint32 Question::getIndex() const
{
    return m_Index;
}

QString Question::print()
{
    switch (m_QuestionType)
    {
        case GameEnums::AIQuestionType_Between:
        {
            return "Index " + QString::number(m_Index) + " Question " +  QString::number(m_MinValue, 'f', 1) + " < value < " + QString::number(m_MaxValue, 'f', 1);
        }
        case GameEnums::AIQuestionType_Equal:
        {
            return "Index " + QString::number(m_Index) + " Question " +  QString::number(m_MinValue, 'f', 1) + " = value";
        }
        case GameEnums::AIQuestionType_Greater:
        {
            return "Index " + QString::number(m_Index) + " Question " +  QString::number(m_MinValue, 'f', 1) + " < value";
        }
        case GameEnums::AIQuestionType_Smaler:
        {
            return "Index " + QString::number(m_Index) + " Question " +  QString::number(m_MinValue, 'f', 1) + " > value";
        }
    }
    return "";
}

void Question::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_MinValue;
    pStream << m_MaxValue;
    pStream << m_Index;
    pStream << static_cast<qint32>(m_QuestionType);
}

void Question::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_MinValue;
    pStream >> m_MaxValue;
    pStream >> m_Index;
    qint32 type = 0;
    pStream >> type;
    m_QuestionType = static_cast<GameEnums::AIQuestionType>(type);
}
