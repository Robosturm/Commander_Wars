#ifndef AI_QUESTION
#define AI_QUESTION

#include "QObject"
#include "QVector"
#include "QString"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/fileserializable.h"

#include "game/GameEnums.h"

class Question;
typedef oxygine::intrusive_ptr<Question> spQuestion;

class Question : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
        Question();
        Question(float value, qint32 index, GameEnums::AIQuestionType QuestionType = GameEnums::AIQuestionType_Equal);
        Question(float minValue, float maxValue, qint32 index, GameEnums::AIQuestionType QuestionType = GameEnums::AIQuestionType_Between);

        virtual ~Question() = default;

        virtual void serializeObject(QDataStream& pStream) const override;
        virtual void deserializeObject(QDataStream& pStream)  override;
        virtual qint32 getVersion() const override
        {
            return 1;
        }
        qint32 getIndex() const;
        QString print();
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
