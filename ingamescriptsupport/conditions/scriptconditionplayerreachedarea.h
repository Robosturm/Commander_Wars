#ifndef SCRIPTCONDITIONPLAYERREACHEDAREA_H
#define SCRIPTCONDITIONPLAYERREACHEDAREA_H

#include "scriptcondition.h"

class ScriptConditionPlayerReachedArea;
typedef oxygine::intrusive_ptr<ScriptConditionPlayerReachedArea> spScriptConditionPlayerReachedArea;

class ScriptConditionPlayerReachedArea : public ScriptCondition
{
    Q_OBJECT
public:
    ScriptConditionPlayerReachedArea();
    /**
     * @brief readCondition
     * @param rStream
     */
    virtual void readCondition(QTextStream& rStream) override;
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
        return tr("Player: ") + QString::number(m_Player) + tr(" reached X:") +
               QString::number(m_x) + " Y: " + QString::number(m_y) +
               tr(" width: ") + QString::number(m_width) + tr(" heigth: ") + QString::number(m_heigth);
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
    virtual qint32 getVersion() override
    {
        return 0;
    }
    /**
     * @brief getPlayer
     * @return
     */
    qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param player
     */
    void setPlayer(const qint32 &player);
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
    qint32 m_Player{0};
    qint32 m_x{0};
    qint32 m_y{0};
    qint32 m_width{0};
    qint32 m_heigth{0};
    QString m_executed;
};

#endif // SCRIPTCONDITIONPLAYERREACHEDAREA_H
