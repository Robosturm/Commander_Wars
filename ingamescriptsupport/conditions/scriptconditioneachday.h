#ifndef SCRIPTCONDITIONEACHDAY_H
#define SCRIPTCONDITIONEACHDAY_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

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
        return tr("Each: ") + QString::number(intervall) + tr(" Start: ")  + QString::number(day) + tr(" Player: ") + QString::number(player + 1);
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
    qint32 intervall{1};
};

#endif // SCRIPTCONDITIONEACHDAY_H
