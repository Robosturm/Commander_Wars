#ifndef SCRIPTEVENTMODIFYUNIT_H
#define SCRIPTEVENTMODIFYUNIT_H

#include "scriptevent.h"

class ScriptEventModifyUnit  : public ScriptEvent
{
public:
    ScriptEventModifyUnit();
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
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() override
    {
        return 0;
    }
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Modify Unit");
    }

private:
    qint32 x{0};
    qint32 y{0};
    qint32 hpChange{0};
    qint32 ammo1Change{0};
    qint32 ammo2Change{0};
    qint32 fuelChange{0};
};

#endif // SCRIPTEVENTMODIFYUNIT_H
