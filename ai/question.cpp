#include "question.h"

Question::Question(float value, qint32 index, GameEnums::AIQuestionType QuestionType)
	: m_MinValue(value),
	  m_MaxValue(value),
      m_Index(index),
      m_QuestionType(QuestionType)
{

}

Question::Question(float minValue, float maxValue, qint32 index, GameEnums::AIQuestionType QuestionType)
	: m_MinValue(minValue),
	  m_MaxValue(maxValue),
      m_Index(index),
      m_QuestionType(QuestionType)
{
}
bool Question::matches(float value)
{
    switch (m_QuestionType)
    {
        case GameEnums::AIQuestionType_Between:
        {
            if ((value >= m_MinValue) &&
                (value <= m_MaxValue))
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
            if (value > m_MaxValue)
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

void Question::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_MinValue;
    pStream << m_MaxValue;
    pStream << static_cast<qint32>(m_QuestionType);
}

void Question::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_MinValue;
    pStream >> m_MaxValue;
    qint32 type = 0;
    pStream >> type;
    m_QuestionType = static_cast<GameEnums::AIQuestionType>(type);
}
