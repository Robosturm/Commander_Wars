#ifndef SCRIPTCONDITIONUNITREACHEDAREA_H
#define SCRIPTCONDITIONUNITREACHEDAREA_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionUnitReachedArea;
typedef oxygine::intrusive_ptr<ScriptConditionUnitReachedArea> spScriptConditionUnitReachedArea;


class ScriptConditionUnitReachedArea : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionUnitReachedArea();
    /**
     * @brief readCondition
     * @param rStream
     */
    virtual void readCondition(QTextStream& rStream, QString line) override;
    /**
     * @brief writeCondition
     * @param rStream
     */
    virtual void writeCondition(QTextStream& rStream) override;
    /**
     * @brief writePreCondition
     * @param rStream
     */
    virtual void writePreCondition(QTextStream& rStream) override;
    /**
     * @brief writePostCondition
     * @param rStream
     */
    virtual void writePostCondition(QTextStream& rStream) override;
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Unit at X: ") + QString::number(m_UnitX) + " Y: " + QString::number(m_UnitY) + tr(" reached X:") +
               QString::number(m_x) + " Y: " + QString::number(m_y) +
               tr(" width: ") + QString::number(m_width) + tr(" heigth: ") + QString::number(m_heigth);
    }
    /**
     * @brief showEditConditin
     */
    virtual void showEditCondition(spScriptEditor pScriptEditor) override;

    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
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
     * @brief getUnitX
     * @return
     */
    qint32 getUnitX() const;
    /**
     * @brief setUnitX
     * @param UnitX
     */
    void setUnitX(const qint32 &UnitX);
    /**
     * @brief getUnitY
     * @return
     */
    qint32 getUnitY() const;
    /**
     * @brief setUnitY
     * @param UnitY
     */
    void setUnitY(const qint32 &UnitY);
    /**
     * @brief getWidth
     * @return
     */
    qint32 getWidth() const;
    /**
     * @brief setWidth
     * @param width
     */
    void setWidth(const qint32 &width);
    /**
     * @brief getHeigth
     * @return
     */
    qint32 getHeigth() const;
    /**
     * @brief setHeigth
     * @param heigth
     */
    void setHeigth(const qint32 &heigth);

private:
    qint32 m_UnitX{0};
    qint32 m_UnitY{0};
    qint32 m_x{0};
    qint32 m_y{0};
    qint32 m_width{1};
    qint32 m_heigth{1};
    QString m_executed;
    QString m_unitID;
};

#endif // SCRIPTCONDITIONPLAYERREACHEDUNIT_H
