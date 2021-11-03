#pragma once

#include <QObject>

class CustomCoBoostInfo : public QObject
{
    Q_OBJECT
public:
    explicit CustomCoBoostInfo() = default;
    virtual ~CustomCoBoostInfo() = default;

    const QStringList &getBonusIcons() const;

    const QStringList &getIconBonus() const;

public slots:
    const QString &getIconId() const;
    void setIconId(const QString &newIconId);

    qint32 getOffensiveBoost() const;
    void setOffensiveBoost(qint32 newOffensiveBoost);

    qint32 getDefensiveBoost() const;
    void setDefensiveBoost(qint32 newDefensiveBoost);

    void addBonusIcon(const QString & id, const QString & value);

    const QString &getLink() const;
    void setLink(const QString &newLink);
private:
    QString m_iconId;
    QString m_link;
    qint32 m_offensiveBoost{0};
    qint32 m_defensiveBoost{0};
    QStringList m_bonusIcons;
    QStringList m_iconBonus;
};

