#ifndef SCRIPTEVENTADDFUNDS_H
#define SCRIPTEVENTADDFUNDS_H


#include "scriptevent.h"

class ScriptEventAddFunds;
using spScriptEventAddFunds = oxygine::intrusive_ptr<ScriptEventAddFunds>;

class ScriptEventAddFunds : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventAddFunds();
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
     * @brief getPlayer
     * @return
     */
    qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param value
     */
    void setPlayer(const qint32 &value);
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Add ") + QString::number(funds) + tr(" Funds to ") + QString::number(player + 1);
    }
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getFunds
     * @return
     */
    qint32 getFunds() const;
    /**
     * @brief setFunds
     * @param value
     */
    void setFunds(const qint32 &value);
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 player{0};
    qint32 funds{0};
};

#endif // SCRIPTEVENTADDFUNDS_H
