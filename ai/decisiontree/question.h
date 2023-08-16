#ifndef AI_QUESTION
#define AI_QUESTION

#include <QObject>
#include <QVector>
#include <QString>

#include "coreengine/fileserializable.h"

#include "game/GameEnums.h"

class Question;
using spQuestion = std::shared_ptr<Question>;

class Question final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
        explicit Question();
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
        bool matches(std::vector<float> &rData);

private:
        float m_MinValue;
        float m_MaxValue;
        qint32 m_Index;
        GameEnums::AIQuestionType m_QuestionType;
};

#endif
