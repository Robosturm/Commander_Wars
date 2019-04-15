#ifndef VERYEASYAI_H
#define VERYEASYAI_H

#include "ai/coreai.h"

class QmlVectorUnit;

class VeryEasyAI : public CoreAI
{
    Q_OBJECT
public:
    VeryEasyAI();

    /**
     * @brief process
     */
    virtual void process() override;
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
    virtual qint32 getVersion() override
    {
        return 1;
    }
protected:
    bool captureBuildings(QmlVectorUnit* pUnits);
    bool fireWithIndirectUnits(QmlVectorUnit* pUnits);
    void finishTurn();
};

#endif // VERYEASYAI_H
