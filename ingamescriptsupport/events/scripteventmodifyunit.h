#ifndef SCRIPTEVENTMODIFYUNIT_H
#define SCRIPTEVENTMODIFYUNIT_H

#include "ingamescriptsupport/events/scriptevent.h"

class ScriptEventModifyUnit;
using spScriptEventModifyUnit = oxygine::intrusive_ptr<ScriptEventModifyUnit>;

class ScriptEventModifyUnit  : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventModifyUnit();
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
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Modify Unit");
    }
    /**
     * @brief getX
     * @return
     */
    qint32 getX() const;
    /**
     * @brief setX
     * @param x
     */
    void setX(const qint32 &x);
    /**
     * @brief getY
     * @return
     */
    qint32 getY() const;
    /**
     * @brief setY
     * @param y
     */
    void setY(const qint32 &y);
    /**
     * @brief getHpChange
     * @return
     */
    qint32 getHpChange() const;
    /**
     * @brief setHpChange
     * @param hpChange
     */
    void setHpChange(const qint32 &hpChange);
    /**
     * @brief getAmmo1Change
     * @return
     */
    qint32 getAmmo1Change() const;
    /**
     * @brief setAmmo1Change
     * @param ammo1Change
     */
    void setAmmo1Change(const qint32 &ammo1Change);
    /**
     * @brief getAmmo2Change
     * @return
     */
    qint32 getAmmo2Change() const;
    /**
     * @brief setAmmo2Change
     * @param ammo2Change
     */
    void setAmmo2Change(const qint32 &ammo2Change);
    /**
     * @brief getFuelChange
     * @return
     */
    qint32 getFuelChange() const;
    /**
     * @brief setFuelChange
     * @param fuelChange
     */
    void setFuelChange(const qint32 &fuelChange);

private:
    qint32 m_x{0};
    qint32 m_y{0};
    qint32 m_hpChange{0};
    qint32 m_ammo1Change{0};
    qint32 m_ammo2Change{0};
    qint32 m_fuelChange{0};
};

#endif // SCRIPTEVENTMODIFYUNIT_H
