#pragma once

#include <QObject>

class CustomCoBoostInfo final : public QObject
{
    Q_OBJECT
public:
    explicit CustomCoBoostInfo();
   virtual ~CustomCoBoostInfo() = default;
    const QStringList &getBonusIcons() const;
    const QStringList &getIconBonus() const;
    Q_INVOKABLE const QString getIconId() const;
    Q_INVOKABLE void setIconId(const QString & newIconId);
    Q_INVOKABLE qint32 getOffensiveBoost() const;
    Q_INVOKABLE void setOffensiveBoost(qint32 newOffensiveBoost);
    Q_INVOKABLE qint32 getDefensiveBoost() const;
    Q_INVOKABLE void setDefensiveBoost(qint32 newDefensiveBoost);
    Q_INVOKABLE void addBonusIcon(const QString & id, const QString & value);
    Q_INVOKABLE const QString getLink() const;
    Q_INVOKABLE void setLink(const QString & newLink);

private:
    QString m_iconId;
    QString m_link;
    qint32 m_offensiveBoost{0};
    qint32 m_defensiveBoost{0};
    QStringList m_bonusIcons;
    QStringList m_iconBonus;
};

Q_DECLARE_INTERFACE(CustomCoBoostInfo, "CustomCoBoostInfo");
