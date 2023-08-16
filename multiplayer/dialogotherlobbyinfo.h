#ifndef DIALOGOTHERLOBBYINFO_H
#define DIALOGOTHERLOBBYINFO_H

#include <QJsonObject>

#include "objects/dialogs/customdialog.h"

class LobbyMenu;
class DialogOtherLobbyInfo;
using spDialogOtherLobbyInfo = std::shared_ptr<DialogOtherLobbyInfo>;

class DialogOtherLobbyInfo final : public CustomDialog
{
public:
    explicit DialogOtherLobbyInfo(LobbyMenu* pBaseMenu, const QJsonObject & objData);
   virtual ~DialogOtherLobbyInfo() = default;

    Q_INVOKABLE qint32 getPreparingAutomatedMatchCount() const;
    Q_INVOKABLE qint32 getRunningAutomatedMatchCount() const;
private:
    const QJsonObject m_otherData;
};

#endif // DIALOGOTHERLOBBYINFO_H
