#ifndef GAMERULE_H
#define GAMERULE_H

#include <QObject>

#include "coreengine/fileserializable.h"

#include "coreengine/scriptvariables.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Player;

class GameRule;
typedef oxygine::intrusive_ptr<GameRule> spGameRule;

class GameRule : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const QString checkbox;
    static const QString spinbox;

    explicit GameRule();
    explicit GameRule(QString ruleID);
    virtual ~GameRule() = default;

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
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
public slots:    
    void setRuleValue(qint32 value, qint32 itemNumber);
    qint32 getInfiniteValue(qint32 itemNumber = 0);
    qint32 getDefaultValue(qint32 itemNumber = 0);
    QStringList getRuleType();
    QString getRuleName(qint32 itemNumber = 0);
    qint32 getRuleValue(qint32 itemNumber);
    QString getRuleDescription(qint32 itemNumber = 0);
    QString getRuleID() const;
    ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
private:
    QString m_RuleID;
    ScriptVariables m_Variables;
};

#endif // VICTORYRULE_H
