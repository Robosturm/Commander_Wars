#pragma once

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionGatheredFunds;
using spScriptConditionGatheredFunds = std::shared_ptr<ScriptConditionGatheredFunds>;

class ScriptConditionGatheredFunds : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionGatheredFunds(GameMap* pMap);
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
        return tr("Gathered funds: ") + QString::number(m_funds);
    }
    /**
     * @brief writePreCondition
     * @param rStream
     */
    virtual void writePreCondition(QTextStream& rStream) override;
    /**
     * @brief writePostCondition
     * @param rStream
     */
    virtual void writePostCondition(QTextStream& rStream) override;
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
    qint32 m_player{0};
    qint32 m_funds{0};
    QString m_Compare = "===";
    QString m_executed;
};

