#ifndef SCRIPTEVENTANIMATION_H
#define SCRIPTEVENTANIMATION_H

#include "scriptevent.h"

class ScriptEventAnimation : public ScriptEvent
{
public:
    ScriptEventAnimation();
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
        return tr("Animation");
    }
private:
    QString sprite;
    qint32 frames{0};
    qint32 x{0};
    qint32 y{0};
    qint32 offsetX{0};
    qint32 offsetY{0};
    QColor color;
    qint32 sleepAfterFinish{0};
    float scaleX;
    float scaleY;
    qint32 delay;
};

#endif // SCRIPTEVENTANIMATION_H
