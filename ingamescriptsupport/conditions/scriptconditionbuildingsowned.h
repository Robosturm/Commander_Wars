#ifndef SCRIPTCONDITIONBUILDINGSOWNED_H
#define SCRIPTCONDITIONBUILDINGSOWNED_H


#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionBuildingsOwned;
typedef oxygine::intrusive_ptr<ScriptConditionBuildingsOwned> spScriptConditionBuildingsOwned;

class ScriptConditionBuildingsOwned : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionBuildingsOwned();

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
        return tr("Buildings Owned: ") + QString::number(m_count) + " Player: " + QString::number(m_player + 1);
    }
    /**
     * @brief showEditConditin
     */
    virtual void showEditCondition(spScriptEditor pScriptEditor) override;
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
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 m_count{0};
    qint32 m_player{0};
    QString m_executed;
};

#endif // SCRIPTCONDITIONBUILDINGSOWNED_H
