#ifndef HEAVYAI_H
#define HEAVYAI_H

#include <QVector>
#include <QTimer>

#include "ai/coreai.h"
#include "game/unitpathfindingsystem.h"

class HeavyAi : public CoreAI
{
    Q_OBJECT
public:
    struct UnitData
    {
        Unit* m_pUnit;
        spUnitPathFindingSystem m_pPfs;
        float m_virtualDamage{0.0f};
    };
    explicit HeavyAi();
    virtual ~HeavyAi() = default;
public slots:
    virtual void process() override;

    void toggleAiPause();
private:
    void initUnits(QmlVectorUnit* pUnits, QVector<UnitData> & units);
    void updateUnits();
    void updateUnits(QVector<UnitData> & units);
private:
    QVector<UnitData> m_enemyUnits;
    QVector<UnitData> m_ownUnits;
    QVector<QPoint> m_updatePoints;

    QmlVectorUnit* m_pUnits = nullptr;
    QmlVectorUnit* m_pEnemyUnits = nullptr;

    QTimer m_timer;
    bool m_pause{false};
};

#endif // HEAVYAI_H
