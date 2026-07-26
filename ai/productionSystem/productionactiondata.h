#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <memory>

class GameMap;
class ProductionActionData;
using spProductionActionData = std::shared_ptr<ProductionActionData>;

class ProductionActionData final : public QObject
{
    Q_OBJECT
public:
    explicit ProductionActionData(GameMap* pMap, qint32 x, qint32 y, QString actionId);
    Q_INVOKABLE void addData(QString text, QString unitId, QString icon, qint32 transactionCost = 0, bool enabled = true);
    Q_INVOKABLE qint32 getX() const;
    Q_INVOKABLE qint32 getY() const;
    Q_INVOKABLE QString getActionId() const;
    Q_INVOKABLE bool getActionAvailable() const;
    Q_INVOKABLE QStringList getUnitIds() const;
    Q_INVOKABLE QVector<qint32> getTransactionCosts() const;
    Q_INVOKABLE QVector<qint32> getStrategicValues() const;
    Q_INVOKABLE QVector<bool> getEnabledList() const;
    void setActionAvailable(bool actionAvailable);
    bool validData() const;

private:
    GameMap* m_pMap{nullptr};
    qint32 m_x{-1};
    qint32 m_y{-1};
    QString m_actionId;
    bool m_actionAvailable{false};
    QStringList m_unitIds;
    QVector<qint32> m_transactionCosts;
    QVector<qint32> m_strategicValues;
    QVector<bool> m_enabledList;
};
