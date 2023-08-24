#ifndef SCRIPTCONDITIONBUILDINGDESTROYED_H
#define SCRIPTCONDITIONBUILDINGDESTROYED_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionBuildingDestroyed;
using spScriptConditionBuildingDestroyed = std::shared_ptr<ScriptConditionBuildingDestroyed>;

class ScriptConditionBuildingDestroyed : public ScriptCondition
{
    Q_OBJECT
public:
    explicit ScriptConditionBuildingDestroyed(GameMap* pMap);

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
        return  QString(tr("Building Destroyed X: %1 Y: %2")).arg(m_x).arg(m_y);
    }
    /**
     * @brief showEditConditin
     */
    virtual void showEditCondition(spScriptEditor pScriptEditor) override;
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
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 m_x{0};
    qint32 m_y{0};
    QString m_executed;
};

#endif // SCRIPTCONDITIONBUILDINGDESTROYED_H
