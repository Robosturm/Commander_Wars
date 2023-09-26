#pragma once

#include <QObject>
#include <QJsonObject>

#include "objects/dialogs/customdialog.h"

class LobbyMenu;
class DialogAutoMatches;
using spDialogAutoMatches = std::shared_ptr<DialogAutoMatches>;

class DialogAutoMatches final : public CustomDialog
{
public:
    explicit DialogAutoMatches(LobbyMenu *pBaseMenu, const QJsonObject &objData);
    virtual ~DialogAutoMatches() = default;

private:

private:
};

Q_DECLARE_INTERFACE(DialogAutoMatches, "DialogAutoMatches");