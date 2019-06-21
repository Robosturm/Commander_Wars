#ifndef SCRIPTCONDITIONSTARTOFTURN_H
#define SCRIPTCONDITIONSTARTOFTURN_H

#include "scriptcondition.h"

class ScriptConditionStartOfTurn;
typedef oxygine::intrusive_ptr<ScriptConditionStartOfTurn> spScriptConditionStartOfTurn;

class ScriptConditionStartOfTurn : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionStartOfTurn();
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
};

#endif // SCRIPTCONDITIONSTARTOFTURN_H
