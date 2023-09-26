#ifndef GAMERULE_H
#define GAMERULE_H

#include <QObject>

#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/jsthis.h"

class Player;

class GameRule;
using spGameRule = std::shared_ptr<GameRule>;

class GameRule final : public QObject, public FileSerializable, public JsThis
{
    Q_OBJECT
public:
    static const char* const checkbox;
    static const char* const spinbox;

    explicit GameRule();
    explicit GameRule(QString ruleID);
    virtual ~GameRule() = default;

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
    Q_INVOKABLE void setRuleValue(qint32 value, qint32 itemNumber);
    Q_INVOKABLE qint32 getInfiniteValue(qint32 itemNumber = 0);
    Q_INVOKABLE qint32 getDefaultValue(qint32 itemNumber = 0);
    Q_INVOKABLE QStringList getRuleType();
    Q_INVOKABLE QString getRuleName(qint32 itemNumber = 0);
    Q_INVOKABLE qint32 getRuleValue(qint32 itemNumber);
    Q_INVOKABLE QString getRuleDescription(qint32 itemNumber = 0);
    Q_INVOKABLE QString getRuleID() const;
    Q_INVOKABLE ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
private:
    QString m_RuleID;
    ScriptVariables m_Variables;
};

Q_DECLARE_INTERFACE(GameRule, "GameRule");

#endif // VICTORYRULE_H
