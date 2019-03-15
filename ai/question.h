#ifndef AI_QUESTION
#define AI_QUESTION

#include "QObject"
#include "QVector"

#include "oxygine-framework.h"

#include "coreengine/fileserializable.h"

#include "game/GameEnums.h"

class Question;
typedef oxygine::intrusive_ptr<Question> spQuestion;

class Question : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
        Question() = default;
        Question(float value, qint32 index, GameEnums::AIQuestionType QuestionType = GameEnums::AIQuestionType_Equal);
        Question(float minValue, float maxValue, qint32 index, GameEnums::AIQuestionType QuestionType = GameEnums::AIQuestionType_Between);

        virtual ~Question() = default;

        virtual void serialize(QDataStream& pStream)  override;
        virtual void deserialize(QDataStream& pStream)  override;
        virtual qint32 getVersion() override
        {
            return 1;
        }
public slots:
        bool matches(float value);
        bool matches(QVector<float> &rData);
private:
        float m_MinValue;
        float m_MaxValue;
        qint32 m_Index;
        GameEnums::AIQuestionType m_QuestionType;
};

#endif
