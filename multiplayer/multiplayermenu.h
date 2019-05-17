#ifndef MULTIPLAYERMENU_H
#define MULTIPLAYERMENU_H

#include "memory.h"

#include <QObject>
#include "oxygine-framework.h"

#include "menue/mapselectionmapsmenue.h"
#include "network/NetworkInterface.h"

class Multiplayermenu : public MapSelectionMapsMenue
{
    Q_OBJECT
public:
    explicit Multiplayermenu(QString adress, bool host);
    virtual ~Multiplayermenu() = default;

signals:

public slots:
    void playerJoined(std::shared_ptr<QTcpSocket> pSocket);
    // general slots
    virtual void slotButtonBack() override;
    virtual void slotButtonNext() override;
    virtual void startGame() override;

    void disconnected(std::shared_ptr<QTcpSocket> pSocket);
private:
    bool m_Host{false};
    spNetworkInterface m_NetworkInterface;
    oxygine::spTextField m_pHostAdresse;
};

#endif // MULTIPLAYERMENU_H
