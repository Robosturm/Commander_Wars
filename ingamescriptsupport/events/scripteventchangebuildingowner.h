#ifndef SCRIPTEVENTCHANGEBUILDINGOWNER_H
#define SCRIPTEVENTCHANGEBUILDINGOWNER_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventChangeBuildingOwner : public ScriptEventGeneric
{
public:
    ScriptEventChangeBuildingOwner();
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

#endif // SCRIPTEVENTCHANGEBUILDINGOWNER_H
