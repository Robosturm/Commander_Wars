#ifndef PLAYERSELECTIONDIALOG_H
#define PLAYERSELECTIONDIALOG_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/playerselection.h"

class GameMap;
class PlayerSelectionDialog;
using spPlayerSelectionDialog = std::shared_ptr<PlayerSelectionDialog>;

class PlayerSelectionDialog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PlayerSelectionDialog(GameMap* pMap);
    virtual ~PlayerSelectionDialog() = default;
signals:
    void sigPlayersChanged();
private slots:
    void remove();
private:
    oxygine::spButton m_OkButton;
    spPlayerSelection m_pPlayerSelection;
};

#endif // PLAYERSELECTIONDIALOG_H
