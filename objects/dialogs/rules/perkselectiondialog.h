#ifndef PERKSELECTIONDIALOG_H
#define PERKSELECTIONDIALOG_H

#include <QObject>
#include "game/player.h"

#include "objects/base/panel.h"
#include "objects/perkselection.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/checkbox.h"
#include "objects/base/label.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

class GameMap;
class PerkSelectionDialog;
using spPerkSelectionDialog = std::shared_ptr<PerkSelectionDialog>;

class PerkSelectionDialog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
    static const char* const SELECT_FILEPATH;
    static const char* const BANN_FILEPATH;
public:
    explicit PerkSelectionDialog(GameMap* pMap, Player* pPlayer, bool banning, QStringList hiddenList);
signals:
    void sigCancel();
    void sigFinished();
    void editFinished(QStringList list);
    void sigToggleAll(bool toggle);
    void sigShowSavePerklist();
    void sigShowDeleteBannlist();
    void sigDeleteBannlist(const QString file);
    void sigDoSaveBannlist(const QString filename, const QString path);
    void sigSelectRandomPerks();
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
    /**
     * @brief doSaveBannlist
     * @param filename
     * @param path
     */
    void doSaveBannlist(const QString filename, const QString path);
    /**
     * @brief showDeleteBannlist
     */
    void showDeleteBannlist();
    /**
     * @brief deleteBannlist
     * @param file
     */
    void deleteBannlist(const QString file);
    /**
     * @brief perkViewUpdated
     */
    void perkViewUpdated();
private:
    QStringList getNameList(QString path);
    void updatePredefinedList();
    QString getFilepath() const;
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_pSave;
    oxygine::spBox9Sprite m_pSpriteBox;
    spLabel m_perkInfo;
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
