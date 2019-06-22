#ifndef SCRIPTEVENTDEFEATPLAYER_H
#define SCRIPTEVENTDEFEATPLAYER_H

#include <QObject>

#include "scriptevent.h"

class ScriptEventDefeatPlayer : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventDefeatPlayer();
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

private:
    qint32 player{0};
};

#endif // SCRIPTEVENTDEFEATPLAYER_H
