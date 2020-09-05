#ifndef SCRIPTEVENTMODIFYVARIABLE_H
#define SCRIPTEVENTMODIFYVARIABLE_H


#include "scriptevent.h"

class ScriptEventModifyVariable : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventModifyVariable();
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream) override;
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) override;
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Modify Variable");
    }
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    QString m_variable{"myName"};
    QString m_Modifier{"+"};
    bool m_CampaignVariable{false};
    qint32 m_value{0};
};

#endif // SCRIPTEVENTADDFUNDS_H
