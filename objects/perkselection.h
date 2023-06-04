#ifndef PERKSELECTION_H
#define PERKSELECTION_H

#include <QObject>
#include <QVector>

#include "objects/base/checkbox.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "game/co.h"

class GameMap;
class PerkSelection;
using spPerkSelection = oxygine::intrusive_ptr<PerkSelection>;

class PerkSelection final : public QObject, public oxygine::Actor
{
    Q_OBJECT
    struct PerkGroup
    {
        QString name;
        QVector<qint32> perks;
    };

public:
    explicit PerkSelection(CO* pCO, qint32 width, qint32 maxPerkCosts, qint32 maxPerkCount, bool banning, QStringList hiddenList, GameMap* pMap);
    ~PerkSelection() = default;
    void updatePerksView(CO* pCO);
    qint32 getPerkScore(const QStringList & perks) const;
    qint32 getPerkScore(const QString & perk) const;
signals:
    void sigUpdatePerkCount();
    void sigViewPerkUpdate();
public slots:
    /**
     * @brief getHiddenPerks
     * @return
     */
    QStringList getHiddenPerks() const;
    /**
     * @brief setHiddenPerks
     * @param hiddenPerks
     */
    void setHiddenPerks(const QStringList &hiddenPerks);
    /**
     * @brief updatePerkCount
     */
    void updatePerkCount();
    /**
     * @brief getPerks
     * @return
     */
    QStringList getPerks() const;
    /**
     * @brief setPerks
     * @param perks
     */
    void setPerks(const QStringList &perks);
    /**
     * @brief toggleAll
     * @param toggle
     */
    void toggleAll(bool toggle);
    /**
     * @brief selectRandomPerks
     */
    void selectRandomPerks(bool fill);
private:
    QVector<PerkGroup> getPerksGrouped();
    bool getPerkEnabled(QString perkId);
private:
    CO* m_pCO{nullptr};
    QVector<spCheckbox> m_Checkboxes;
    QStringList m_perkIds;
    qint32 m_maxPerkCosts{0};
    qint32 m_maxPerkCount{0};
    bool m_banning{false};
    QStringList m_perks;
    QStringList m_hiddenPerks;
    GameMap* m_pMap{nullptr};
};

#endif // PERKSELECTION_H
