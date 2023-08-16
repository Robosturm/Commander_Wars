#ifndef NETWORKGAMEDATAVIEW_H
#define NETWORKGAMEDATAVIEW_H

#include <QObject>
#include <QImage>
#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"

#include "ui_reader/createdgui.h"

#include "network/networkgamedata.h"

class NetworkGameDataView;
using spNetworkGameDataView = std::shared_ptr<NetworkGameDataView>;

class NetworkGameDataView : public CreatedGui
{
    Q_OBJECT
public:
    explicit NetworkGameDataView(NetworkGameData & data);
    virtual ~NetworkGameDataView() = default;
    virtual oxygine::spActor loadCustomId(const QString & item, qint32 x, qint32 y, bool enabled, bool visible, float scale,
                                          const QString & id, const QString & tooltip, const QString & onEvent,
                                          UiFactory* pFactoty, CreatedGui* pMenu, qint32 loopIdx, qint32 & scaledWidth, qint32 & scaledHeight) override;
    Q_INVOKABLE void close();
    Q_INVOKABLE QString getMapName() const;
    Q_INVOKABLE QString getDescription() const;
    Q_INVOKABLE qint32 getPlayers() const;
    Q_INVOKABLE qint32 getMaxPlayers() const;
    Q_INVOKABLE QString getPlayerName(qint32 player);
    Q_INVOKABLE bool getPlayerIsOnline(qint32 player);
    Q_INVOKABLE qint32 getModCount() const;
    Q_INVOKABLE QString getModName(qint32 mod) const;
    Q_INVOKABLE qint32 getObservers() const;
    Q_INVOKABLE qint32 getMaxObservers() const;
private:
    void getMinimapImage(QImage & img, NetworkGameData & data);
private:
    oxygine::spSingleResAnim m_minimapImage;
    NetworkGameData & m_data;
};

Q_DECLARE_INTERFACE(NetworkGameDataView, "NetworkGameDataView");

#endif // NETWORKGAMEDATAVIEW_H
