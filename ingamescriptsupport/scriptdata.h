#ifndef SCRIPTDATA_H
#define SCRIPTDATA_H

#include <QObject>

#include "qvector.h"

#include "oxygine-framework.h"

#include "scriptconditionvictory.h"

class ScriptData;
typedef oxygine::intrusive_ptr<ScriptData> spScriptData;

class ScriptData : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const QString scriptStart;
    static const QString scriptEnd;
    static const QString immediateStart;
    static const QString victory;
    static const QString turnStart;

    explicit ScriptData();
    /**
     * @brief readScript
     * @param rStream
     */
    void readScript(QTextStream& rStream);
    /**
     * @brief writeScript
     * @param rStream
     */
    void writeScript(QTextStream& rStream);
    /**
     * @brief readData
     * @param id
     * @param rStream
     */
    void readData(QString id, QTextStream& rStream, QString& customCode, QVector<spScriptCondition>* rVector);

    /**
     * @brief addVictoryCondition
     * @param type
     */
    spScriptCondition addVictoryCondition();
    /**
     * @brief addDayCondition
     * @param type
     */
    spScriptCondition addDayCondition(ScriptCondition::ConditionType type);
    /**
     * @brief getVictoryCondition
     * @param index
     * @return
     */
    spScriptCondition getVictoryCondition(qint32 index);
    /**
     * @brief getVictoryConditionSize
     * @return
     */
    qint32 getVictoryConditionSize()
    {
        return m_Victory.size();
    }
    /**
     * @brief getDayCondition
     * @param index
     * @return
     */
    spScriptCondition getDayCondition(qint32 index);
    /**
     * @brief getDayConditionSize
     * @return
     */
    qint32 getDayConditionSize()
    {
        return m_DayConditions.size();
    }
    /**
     * @brief clearData
     */
    void clearData();
    /**
     * @brief getStartMode
     * @return
     */
    bool getStartMode() const;
    /**
     * @brief setStartMode
     * @param value
     */
    void setStartMode(bool value);

private:
    QVector<spScriptCondition> m_Victory;
    QString customVictoryCode;
    QVector<spScriptCondition> m_DayConditions;
    QString customStartOfTurnCode;
    QString customCode;
    bool startMode{false};

};

#endif // SCRIPTDATA_H
