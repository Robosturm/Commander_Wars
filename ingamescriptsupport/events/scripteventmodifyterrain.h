#ifndef SCRIPTEVENTMODIFYTERRAIN_H
#define SCRIPTEVENTMODIFYTERRAIN_H

#include "scriptevent.h"

class ScriptEventModifyTerrain  : public ScriptEvent
{
public:
    ScriptEventModifyTerrain();
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
    QString newTerrainID;
};

#endif // SCRIPTEVENTMODIFYTERRAIN_H
