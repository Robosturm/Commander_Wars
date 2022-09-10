#ifndef SCRIPTDATA_H
#define SCRIPTDATA_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

#include "conditions/scriptcondition.h"

class GameMap;
class ScriptData;
using spScriptData = oxygine::intrusive_ptr<ScriptData>;

class ScriptData final : public QObject, public oxygine::ref_counter
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

    explicit ScriptData(GameMap* pMap);
    ~ScriptData() = default;
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
    QString m_customVictoryCode;
    QVector<spScriptCondition> m_DayConditions;
    QString m_customStartOfTurnCode;
    QVector<spScriptCondition> m_ActionConditions;
    QString m_customActionConditions;

    QString m_customCode;
    bool m_startMode{false};
    GameMap* m_pMap{nullptr};

    static quint32 m_variableCounter;
};

#endif // SCRIPTDATA_H
