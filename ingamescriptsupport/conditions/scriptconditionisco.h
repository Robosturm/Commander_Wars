#pragma once

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionIsCo;
using spScriptConditionIsCo = oxygine::intrusive_ptr<ScriptConditionIsCo>;

class ScriptConditionIsCo final : public ScriptCondition
{
    Q_OBJECT
public:
    explicit ScriptConditionIsCo(GameMap* pMap);
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
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Is co player: ") + QString::number(m_player) +  tr(" co index: ") + QString::number(m_playerCo) +  tr(" equal ") + m_coid;
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
    qint32 m_player{0};
    qint32 m_playerCo{0};
    QString m_coid{"CO_ANDY"};
    QString m_executed;
};

