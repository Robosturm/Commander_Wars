#ifndef SCRIPTCONDITIONUNITSDESTROYED_H
#define SCRIPTCONDITIONUNITSDESTROYED_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionUnitsDestroyed;
typedef oxygine::intrusive_ptr<ScriptConditionUnitsDestroyed> spScriptConditionUnitsDestroyed;

class ScriptConditionUnitsDestroyed : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionUnitsDestroyed();
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
        return tr("Units killed: ") + QString::number(m_count) + tr(" by Player") + QString::number(m_player + 1);
    }
    /**
     * @brief showEditConditin
     */
    virtual void showEditCondition(spScriptEditor pScriptEditor) override;
    /**
     * @brief getPlayer
     * @return
     */
    qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param player
     */
    void setPlayer(const qint32 &player);
    /**
     * @brief getCount
     * @return
     */
    qint32 getCount() const;
    /**
     * @brief setCount
     * @param count
     */
    void setCount(const qint32 &count);
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
    qint32 m_count{1};
    QString m_executed;
};

#endif // SCRIPTCONDITIONUNITSDESTROYED_H
