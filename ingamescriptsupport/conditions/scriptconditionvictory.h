#ifndef SCRIPTCONDITIONVICTORY_H
#define SCRIPTCONDITIONVICTORY_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionVictory;
typedef oxygine::intrusive_ptr<ScriptConditionVictory> spScriptConditionVictory;

class ScriptConditionVictory : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionVictory();

    qint32 getTeam() const;
    void setTeam(const qint32 &value);
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
        return tr("Victory Team: ") + QString::number(team + 1);
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
    qint32 team{0};
};

#endif // SCRIPTCONDITIONVICTORY_H
