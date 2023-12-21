#pragma once

#include <QObject>

#include "objects/dialogs/customdialog.h"
#include "mapsupport/mapfilter.h"
#include "network/filepeer.h"

class LobbyMenu;
class DialogSelectDownloadRecord;
using spDialogSelectDownloadRecord = std::shared_ptr<DialogSelectDownloadRecord>;


class DialogSelectDownloadRecord : public CustomDialog
{
    Q_OBJECT
public:
    DialogSelectDownloadRecord(LobbyMenu* pBaseMenu);
    virtual ~DialogSelectDownloadRecord() = default;
    Q_INVOKABLE void showMapFilter();
    Q_INVOKABLE qint32 getCurrentPage() const;
    Q_INVOKABLE qint32 getPageCount();
    Q_INVOKABLE QString getRecordPath(qint32 mapIndex);
    Q_INVOKABLE QString getMapName(qint32 mapIndex);
    Q_INVOKABLE QString getMapAuthor(qint32 mapIndex);
    Q_INVOKABLE qint32 getMapPlayers(qint32 mapIndex);
    Q_INVOKABLE qint32 getMapWidth(qint32 mapIndex);
    Q_INVOKABLE qint32 getMapHeight(qint32 mapIndex);
    Q_INVOKABLE qint64 getMapFlags(qint32 mapIndex);
    Q_INVOKABLE qint32 getRecordCounts();
    Q_INVOKABLE void changeCurrentPage(qint32 direction);
    Q_INVOKABLE void changeCurrentPageToEnd(bool start);
    Q_INVOKABLE void downloadRecord(qint32 recordIndex);
    Q_INVOKABLE QString getRecordHeader(qint32 recordIndex);
private slots:
    void filterChanged();
    void receivedRecordData(const QJsonObject &objData);
    void receivedChunk(qint64 sendBytes, qint64 size, bool atEnd);
private:
    LobbyMenu* m_pBaseMenu{nullptr};
    qint32 m_currentStartIndex{0};
    MapFilter m_mapFilter;
    QJsonObject m_recordData;
    spFilePeer m_filePeer;
};

Q_DECLARE_INTERFACE(DialogSelectDownloadRecord, "DialogSelectDownloadRecord");
