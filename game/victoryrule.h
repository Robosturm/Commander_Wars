#ifndef VICTORYRULE_H
#define VICTORYRULE_H

#include <QObject>

#include "coreengine/fileserializable.h"

#include "coreengine/scriptvariables.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Player;

class VictoryRule;
typedef oxygine::intrusive_ptr<VictoryRule> spVictoryRule;

class VictoryRule : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const QString checkbox;
    static const QString spinbox;

    explicit VictoryRule();
    explicit VictoryRule(QString ruleID);
    virtual ~VictoryRule() = default;

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
    qint32 getRuleProgress(Player* pPlayer);
    QString getRuleID() const;
    ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    void checkDefeat();
private:
    QString m_RuleID;
    ScriptVariables m_Variables;
};

#endif // VICTORYRULE_H
