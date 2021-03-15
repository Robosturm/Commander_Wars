#ifndef GAMERECORDSPECIALEVENT_H
#define GAMERECORDSPECIALEVENT_H

#include "qobject.h"

#include "game/GameEnums.h"

#include "coreengine/fileserializable.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class SpecialEvent;
typedef oxygine::intrusive_ptr<SpecialEvent> spSpecialEvent;

class SpecialEvent : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit SpecialEvent();
    explicit SpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event);

    qint32 getOwner() const;
    qint32 getDay() const;
    GameEnums::GameRecord_SpecialEvents getEvent() const;

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 1;
    }
private:
    qint32 m_Player{-1};
    qint32 m_Day{-1};
    GameEnums::GameRecord_SpecialEvents m_Event{GameEnums::GameRecord_SpecialEvents_Power};
};

#endif // SPECIALEVENT_H
