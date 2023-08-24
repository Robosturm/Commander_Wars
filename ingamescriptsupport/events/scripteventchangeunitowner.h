#ifndef SCRIPTEVENTCHANGEUNITOWNER_H
#define SCRIPTEVENTCHANGEUNITOWNER_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventChangeUnitOwner;
using spScriptEventChangeUnitOwner = std::shared_ptr<ScriptEventChangeUnitOwner>;

class ScriptEventChangeUnitOwner final : public ScriptEventGeneric
{
    Q_OBJECT
public:
    explicit ScriptEventChangeUnitOwner(GameMap* pMap);
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
