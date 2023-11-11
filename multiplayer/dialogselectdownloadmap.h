#pragma once

#include <QObject>
#include <QImage>
#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"

#include "objects/dialogs/customdialog.h"
#include "mapsupport/mapfilter.h"

class LobbyMenu;

class DialogSelectDownloadMap : public CustomDialog
{
    Q_OBJECT
public:
    DialogSelectDownloadMap(LobbyMenu* pBaseMenu);
    virtual ~DialogSelectDownloadMap() = default;    
    virtual oxygine::spActor loadCustomId(const QString & item, qint32 x, qint32 y, bool enabled, bool visible, float scale,
                                          const QString & id, const QString & tooltip, const QString & onEvent,
                                          UiFactory* pFactoty, CreatedGui* pMenu, qint32 loopIdx, qint32 & scaledWidth, qint32 & scaledHeight) override;
    Q_INVOKABLE void showMapFilter();
    Q_INVOKABLE qint32 getCurrentPage() const;
    Q_INVOKABLE void setCurrentPage(qint32 newCurrentPage);
    Q_INVOKABLE void downloadMap(qint32 mapIndex);
    Q_INVOKABLE QString getMapPath(qint32 mapIndex);
    Q_INVOKABLE QString getMapName(qint32 mapIndex);
    Q_INVOKABLE QString getMapAuthor(qint32 mapIndex);
    Q_INVOKABLE qint32 getMapPlayers(qint32 mapIndex);
    Q_INVOKABLE qint32 getMapWidth(qint32 mapIndex);
    Q_INVOKABLE qint32 getMapHeight(qint32 mapIndex);
    Q_INVOKABLE qint64 getMapFlags(qint32 mapIndex);
    Q_INVOKABLE qint32 getMapCounts();
private slots:
    void filterChanged();
    void receivedMapData(const QJsonObject &objData);
private:
    LobbyMenu* m_pBaseMenu{nullptr};
    qint32 m_currentPage{0};
    MapFilter m_mapFilter;
    QJsonObject m_mapData;
    std::vector<QImage> m_minimapImages;
    std::vector<oxygine::spSingleResAnim> m_minimapSprites;
};

