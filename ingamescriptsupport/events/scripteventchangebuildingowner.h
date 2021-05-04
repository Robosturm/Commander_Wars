#ifndef SCRIPTEVENTCHANGEBUILDINGOWNER_H
#define SCRIPTEVENTCHANGEBUILDINGOWNER_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventChangeBuildingOwner;
using spScriptEventChangeBuildingOwner = oxygine::intrusive_ptr<ScriptEventChangeBuildingOwner>;

class ScriptEventChangeBuildingOwner : public ScriptEventGeneric
{
    Q_OBJECT
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
