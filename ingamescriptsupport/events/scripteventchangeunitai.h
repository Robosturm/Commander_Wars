#ifndef SCRIPTEVENTCHANGEUNITAI_H
#define SCRIPTEVENTCHANGEUNITAI_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventChangeUnitAI : public ScriptEventGeneric
{
public:
    ScriptEventChangeUnitAI();
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

#endif // SCRIPTEVENTCHANGEUNITAI_H
