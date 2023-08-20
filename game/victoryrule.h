#ifndef VICTORYRULE_H
#define VICTORYRULE_H

#include <QObject>

#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"

class Player;
class GameMap;
class VictoryRule;
using spVictoryRule = std::shared_ptr<VictoryRule>;

class VictoryRule final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    static const char* const checkbox;
    static const char* const spinbox;

    explicit VictoryRule(GameMap* pMap);
    explicit VictoryRule(QString ruleID, GameMap* pMap);
    ~VictoryRule() = default;

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief serializeObject
     * @param pStream
     * @param forHash
     */
    void serializeObject(QDataStream& pStream, bool forHash) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 1;
    }
    /**
     * @brief init
     */
    void init();
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    Q_INVOKABLE void setRuleValue(qint32 value, qint32 itemNumber);
    Q_INVOKABLE qint32 getInfiniteValue(qint32 itemNumber = 0);
    Q_INVOKABLE qint32 getDefaultValue(qint32 itemNumber = 0);
    Q_INVOKABLE QStringList getRuleType();
    Q_INVOKABLE QString getRuleName(qint32 itemNumber = 0);
    Q_INVOKABLE qint32 getRuleValue(qint32 itemNumber);
    Q_INVOKABLE QString getRuleDescription(qint32 itemNumber = 0);
    Q_INVOKABLE qint32 getRuleProgress(Player* pPlayer);
    Q_INVOKABLE QString getRuleID() const;
    Q_INVOKABLE ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    Q_INVOKABLE void checkDefeat();
private:
    QString m_RuleID;
    ScriptVariables m_Variables;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(VictoryRule, "VictoryRule");

#endif // VICTORYRULE_H
