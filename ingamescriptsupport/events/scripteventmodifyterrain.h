#ifndef SCRIPTEVENTMODIFYTERRAIN_H
#define SCRIPTEVENTMODIFYTERRAIN_H

#include "ingamescriptsupport/events/scriptevent.h"

class ScriptEventModifyTerrain;
using spScriptEventModifyTerrain = oxygine::intrusive_ptr<ScriptEventModifyTerrain>;

class ScriptEventModifyTerrain  : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventModifyTerrain();
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
     * @brief getNewTerrainID
     * @return
     */
    QString getNewTerrainID() const;
    /**
     * @brief setNewTerrainID
     * @param newTerrainID
     */
    void setNewTerrainID(const QString &newTerrainID);

private:
    qint32 m_x{0};
    qint32 m_y{0};
    QString m_newTerrainID;
};

#endif // SCRIPTEVENTMODIFYTERRAIN_H
