#ifndef SCRIPTCONDITIONSTARTOFTURN_H
#define SCRIPTCONDITIONSTARTOFTURN_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

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
    virtual void readCondition(QTextStream& rStream, QString line) override;
    /**
     * @brief writeCondition
     * @param rStream
     */
    virtual void writeCondition(QTextStream& rStream) override;
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Day: ") + QString::number(day) + tr(" Player: ") + QString::number(player + 1);
    }
    /**
     * @brief showEditConditin
     */
    virtual void showEditCondition(spScriptEditor pScriptEditor) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 day{1};
    qint32 player{0};
};

#endif // SCRIPTCONDITIONSTARTOFTURN_H
