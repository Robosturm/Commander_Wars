#ifndef NETWORKGAMEDATAVIEW_H
#define NETWORKGAMEDATAVIEW_H

#include <QObject>
#include <QImage>
#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "network/networkgamedata.h"

class NetworkGameDataView;
using spNetworkGameDataView = oxygine::intrusive_ptr<NetworkGameDataView>;

class NetworkGameDataView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit NetworkGameDataView(NetworkGameData & data);
    virtual ~NetworkGameDataView() = default;
signals:
    /**
     * @brief sigFinished
     */
    void sigFinished();
private slots:
    void remove();
private:
    void getMinimapImage(QImage & img, NetworkGameData & data);
private:
    oxygine::spSingleResAnim m_minimapImage;
};

#endif // NETWORKGAMEDATAVIEW_H
