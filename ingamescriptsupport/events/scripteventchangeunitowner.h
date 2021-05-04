#ifndef SCRIPTEVENTCHANGEUNITOWNER_H
#define SCRIPTEVENTCHANGEUNITOWNER_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventChangeUnitOwner;
using spScriptEventChangeUnitOwner = oxygine::intrusive_ptr<ScriptEventChangeUnitOwner>;

class ScriptEventChangeUnitOwner : public ScriptEventGeneric
{
    Q_OBJECT
public:
    ScriptEventChangeUnitOwner();
    /**
     * @brief removeCustomStart
     * @param text
     */
    virtual void removeCustomStart(QString& text) override;
    /**
     * @brief writeCustomStart
     */
    virtual void writeCustomStart(QTextStream& stream) override;
};

#endif // SCRIPTEVENTCHANGEUNITOWNER_H
