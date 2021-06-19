#ifndef SCRIPTEVENTSPAWNUNIT_H
#define SCRIPTEVENTSPAWNUNIT_H

#include "ingamescriptsupport/events/scriptevent.h"

class ScriptEventSpawnUnit;
using spScriptEventSpawnUnit = oxygine::intrusive_ptr<ScriptEventSpawnUnit>;

class ScriptEventSpawnUnit : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventSpawnUnit();
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream, QString line) override;
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) override;
    /**
     * @brief getX
     * @return
     */
    qint32 getX() const;
    /**
     * @brief setX
     * @param value
     */
    void setX(const qint32 &value);
    /**
     * @brief getY
     * @return
     */
    qint32 getY() const;
    /**
     * @brief setY
     * @param value
     */
    void setY(const qint32 &value);
    /**
     * @brief getUnitID
     * @return
     */
    QString getUnitID() const;
    /**
     * @brief setUnitID
     * @param value
     */
    void setUnitID(const QString &value);
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
    /**
     * @brief getRadius
     * @return
     */
    qint32 getRadius() const;
    /**
     * @brief setRadius
     * @param value
     */
    void setRadius(const qint32 &value);
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Spawn Unit");
    }
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 x{0};
    qint32 y{0};
    QString unitID{"INFANTRY"};
    qint32 player{0};
    qint32 radius{0};
};

#endif // SCRIPTEVENTSPAWNUNIT_H
