#ifndef VICTORYRULE_H
#define VICTORYRULE_H

#include <QObject>

#include "coreengine/fileserializable.h"

#include "coreengine/scriptvariables.h"

#include "oxygine-framework.h"

class VictoryRule;
typedef oxygine::intrusive_ptr<VictoryRule> spVictoryRule;

class VictoryRule : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit VictoryRule();
    explicit VictoryRule(QString ruleID);
    virtual ~VictoryRule() = default;

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serialize(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserialize(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 1;
    }

public slots:
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
