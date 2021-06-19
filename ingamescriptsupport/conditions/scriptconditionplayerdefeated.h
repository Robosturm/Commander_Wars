#ifndef SCRIPTCONDITIONPLAYERDEFEATED_H
#define SCRIPTCONDITIONPLAYERDEFEATED_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionPlayerDefeated;
typedef oxygine::intrusive_ptr<ScriptConditionPlayerDefeated> spScriptConditionPlayerDefeated;

class ScriptConditionPlayerDefeated : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionPlayerDefeated();

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
        return tr("Player Defeated: ") + QString::number(m_player + 1);
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
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 m_player{0};
    QString m_executed;
};

#endif // SCRIPTCONDITIONPLAYERDEFEATED_H
