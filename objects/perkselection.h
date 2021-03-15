#ifndef PERKSELECTION_H
#define PERKSELECTION_H

#include <QObject>
#include <QVector>

#include "objects/base/checkbox.h"

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "game/co.h"

class PerkSelection;
typedef oxygine::intrusive_ptr<PerkSelection> spPerkSelection;

class PerkSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PerkSelection(CO* pCO, qint32 width, qint32 maxPerks, bool banning, QStringList hiddenList);

    void updatePerksView(CO* pCO);

signals:
    void sigUpdatePerkCount();
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
    CO* m_pCO{nullptr};
    QVector<spCheckbox> m_Checkboxes;
    qint32 m_maxPerks{0};
    bool m_banning{false};
    QStringList m_perks;
    QStringList m_hiddenPerks;
};

#endif // PERKSELECTION_H
