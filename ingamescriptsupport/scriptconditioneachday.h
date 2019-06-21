#ifndef SCRIPTCONDITIONEACHDAY_H
#define SCRIPTCONDITIONEACHDAY_H

#include "scriptcondition.h"

class ScriptConditionEachDay;
typedef oxygine::intrusive_ptr<ScriptConditionEachDay> spScriptConditionEachDay;

class ScriptConditionEachDay : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionEachDay();

    /**
     * @brief getDay
     * @return
     */
    qint32 getDay() const;
    /**
     * @brief setDay
     * @param value
     */
    void setDay(const qint32 &value);
    /**
     * @brief getPlayer
     * @return
     */
    qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param value
     */
    void setPlayer(const qint32 &value);
    /**
     * @brief getIntervall
     * @return
     */
    qint32 getIntervall() const;
    /**
     * @brief setIntervall
     * @param value
     */
    void setIntervall(const qint32 &value);
    /**
     * @brief readCondition
     * @param rStream
     */
    virtual void readCondition(QTextStream& rStream) override;
    /**
     * @brief writeCondition
     * @param rStream
     */
    virtual void writeCondition(QTextStream& rStream) override;
private:
    qint32 day{1};
    qint32 player{0};
    qint32 intervall{1};
};

#endif // SCRIPTCONDITIONEACHDAY_H
