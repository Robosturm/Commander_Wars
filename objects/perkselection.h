#ifndef PERKSELECTION_H
#define PERKSELECTION_H

#include <QObject>
#include <QVector>

#include "objects/checkbox.h"
#include "oxygine-framework.h"
#include "game/co.h"

class PerkSelection;
typedef oxygine::intrusive_ptr<PerkSelection> spPerkSelection;

class PerkSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PerkSelection(CO* pCO, qint32 width, qint32 maxPerks, bool banning);

    void updatePerksView(CO* pCO);
signals:
    void sigUpdatePerkCount();
public slots:
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
private:
    CO* m_pCO{nullptr};
    QVector<spCheckbox> m_Checkboxes;
    qint32 m_maxPerks{0};
    bool m_banning{false};
    QStringList m_perks;
};

#endif // PERKSELECTION_H
