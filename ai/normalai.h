#ifndef NORMALAI_H
#define NORMALAI_H

#include <QObject>

#include "ai/coreai.h"

#include "game/smartUnit.h"

class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;

class NormalAi : public CoreAI
{
    Q_OBJECT
public:
    explicit NormalAi();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() override
    {
        return 1;
    }
signals:

public slots:
    virtual void process() override;
protected:
    bool isUsingUnit(Unit* pUnit);

    void calcVirtualDamage();
    bool captureBuildings(QmlVectorUnit* pUnits);
private:
    QVector<spUnit> m_EnemyUnits;
    QVector<float> m_VirtualDamage;
};

#endif // NORMALAI_H
