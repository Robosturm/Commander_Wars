#ifndef SCRIPTDATA_H
#define SCRIPTDATA_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "conditions/scriptcondition.h"

class ScriptData;
typedef oxygine::intrusive_ptr<ScriptData> spScriptData;

class ScriptData : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const QString scriptStart;
    static const QString scriptEnd;
    static const QString immediateStart;
    static const QString victoryInfo;
    static const QString victory;
    static const QString actionConditions;
    static const QString turnStart;
    static constexpr auto campaignVariables = "campaignVariables";
    static constexpr auto variables = "variables";

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
    spScriptCondition addVictoryCondition(spScriptCondition condition);
    /**
     * @brief addDayCondition
     * @param type
     */
    spScriptCondition addDayCondition(spScriptCondition condition);
    /**
     * @brief addActionCondition
     * @param type
     * @return
     */
    spScriptCondition addActionCondition(spScriptCondition condition);
    /**
     * @brief removeCondition
     * @param condition
     */
    void removeCondition(spScriptCondition condition);
    /**
     * @brief removeCondition
     * @param data
     * @param condition
     */
    void removeCondition(QVector<spScriptCondition>& data, spScriptCondition condition);
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
     * @brief getDayCondition
     * @param index
     * @return
     */
    spScriptCondition getActionCondition(qint32 index);
    /**
     * @brief getDayConditionSize
     * @return
     */
    qint32 getActionConditionSize()
    {
        return m_ActionConditions.size();
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
    /**
     * @brief getVariableName
     * @return
     */
    static QString getVariableName();
private:
    QVector<spScriptCondition> m_Victory;
    QString customVictoryCode;
    QVector<spScriptCondition> m_DayConditions;
    QString customStartOfTurnCode;
    QVector<spScriptCondition> m_ActionConditions;
    QString customActionConditions;

    QString customCode;
    bool startMode{false};

    static quint32 m_variableCounter;
};

#endif // SCRIPTDATA_H
