#ifndef PERKSELECTIONDIALOG_H
#define PERKSELECTIONDIALOG_H

#include <QObject>
#include "game/player.h"

#include "objects/base/panel.h"
#include "objects/perkselection.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/checkbox.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class GameMap;
class PerkSelectionDialog;
using spPerkSelectionDialog = oxygine::intrusive_ptr<PerkSelectionDialog>;

class PerkSelectionDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PerkSelectionDialog(GameMap* pMap, Player* pPlayer, qint32 maxPerkcount, bool banning, QStringList hiddenList);
protected slots:
    void changeCO(qint32 index);
    /**
     * @brief showSaveBannlist
     */
    void showSavePerklist();
    /**
     * @brief saveBannlist
     */
    void savePerklist(QString filename);
    /**
     * @brief savePerklist
     * @param item
     */
    void setPerkBannlist(qint32 item);
    /**
     * @brief selectRandomPerks
     */
    void selectRandomPerks();
    /**
     * @brief remove
     */
    void remove();
signals:
    void sigCancel();
    void sigFinished();
    void editFinished(QStringList list);
    void sigToggleAll(bool toggle);
    void sigShowSavePerklist();
    void sigSelectRandomPerks();
private:
    QStringList getNameList(QString path);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    oxygine::spButton m_ToggleAll;
    spDropDownmenu m_PredefinedLists;
    spCheckbox m_randomFillCheckbox;
    bool m_toggle{true};
    Player* m_pPlayer{nullptr};
    spPanel m_pPanel;
    spPerkSelection m_pPerkSelection;
    bool m_banning{false};
    GameMap* m_pMap{nullptr};
};

#endif // PERKSELECTIONDIALOG_H
