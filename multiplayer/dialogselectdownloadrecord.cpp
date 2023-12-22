#include "multiplayer/lobbymenu.h"
#include "multiplayer/dialogselectdownloadrecord.h"
#include "multiplayer/networkcommands.h"

#include "coreengine/gameconsole.h"

#include "network/JsonKeys.h"

#include "objects/dialogs/mapSelection/mapselectionfilterdialog.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/loadingscreen.h"

#include <QJsonObject>
#include <QJsonDocument>

static constexpr qint32 ITEMS_PER_PAGE = 10;

DialogSelectDownloadRecord::DialogSelectDownloadRecord(LobbyMenu* pBaseMenu)
    : CustomDialog("", "", pBaseMenu, tr("Close")),
    m_pBaseMenu(pBaseMenu)
{
    m_uiXml = "ui/multiplayer/selectDownloadRecord.xml";
    loadXmlFile(m_uiXml);

    connect(m_pBaseMenu, &LobbyMenu::sigReceivedAvailableRecords, this, &DialogSelectDownloadRecord::receivedRecordData, Qt::QueuedConnection);
}

void DialogSelectDownloadRecord::showMapFilter()
{
    spMapSelectionFilterDialog mapSelectionFilterDialog = MemoryManagement::create<MapSelectionFilterDialog>(&m_mapFilter);
    addChild(mapSelectionFilterDialog);
    connect(mapSelectionFilterDialog.get(), &MapSelectionFilterDialog::sigFinished, this, &DialogSelectDownloadRecord::filterChanged, Qt::QueuedConnection);
}

qint32 DialogSelectDownloadRecord::getCurrentPage() const
{
    return m_currentStartIndex / ITEMS_PER_PAGE;
}

void DialogSelectDownloadRecord::changeCurrentPageToEnd(bool start)
{
    if (start)
    {
        changeCurrentPage(0);
    }
    else
    {
        changeCurrentPage((getPageCount() - 1) * ITEMS_PER_PAGE);
    }
}

void DialogSelectDownloadRecord::changeCurrentPage(qint32 direction)
{
    auto curIdx = m_currentStartIndex;
    if (direction > 0)
    {
        m_currentStartIndex += ITEMS_PER_PAGE;
    }
    else
    {
        m_currentStartIndex -= ITEMS_PER_PAGE;
    }
    if (m_currentStartIndex < 0)
    {
        m_currentStartIndex = 0;
    }
    qint32 pageCount = getPageCount() * ITEMS_PER_PAGE;
    if (m_currentStartIndex >= pageCount)
    {
        m_currentStartIndex = pageCount - 1;
    }
    if (curIdx != m_currentStartIndex)
    {
        filterChanged();
    }
}

qint32 DialogSelectDownloadRecord::getPageCount()
{
    qint32 items = m_recordData.value(JsonKeys::JSONKEY_FOUNDITEMS).toInt();
    qint32 page = items / ITEMS_PER_PAGE;
    if (page % ITEMS_PER_PAGE > 0)
    {
        ++page;
    }
    return page;
}

void DialogSelectDownloadRecord::filterChanged()
{
    QString command = QString(NetworkCommands::REQUESTAVAILABLERECORDS);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data = m_mapFilter.toJson();
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_ITEMSPERPAGE, ITEMS_PER_PAGE);
    data.insert(JsonKeys::JSONKEY_STARTINDEX, m_currentStartIndex);
    m_pBaseMenu->sendCommandToServer(data);
}

void DialogSelectDownloadRecord::receivedRecordData(const QJsonObject &objData)
{
    m_recordData = objData;
    m_currentStartIndex = objData.value(JsonKeys::JSONKEY_STARTINDEX).toInt();
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    CONSOLE_PRINT("Refreshing ui with the received records " + QString::number(recordArray.size()), GameConsole::eDEBUG);
    refreshUi();
}

void DialogSelectDownloadRecord::downloadRecord(qint32 recordIndex)
{
    m_filePeer = MemoryManagement::create<FilePeer>(m_pBaseMenu->getTcpClient(), getRecordPath(recordIndex), 0);
    m_filePeer->startDownload(NetworkCommands::RECORDFILEDOWNLOADREQUEST);
    connect(m_filePeer.get(), &FilePeer::sigDownloadInfo, this, &DialogSelectDownloadRecord::receivedChunk, Qt::QueuedConnection);
    connect(m_pBaseMenu, &LobbyMenu::sigReceivedFilePacket, m_filePeer.get(), &FilePeer::receivedPacket, Qt::QueuedConnection);
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();
    pLoadingScreen->setProgress("Downloading record from server...", 0);
}

void DialogSelectDownloadRecord::receivedChunk(qint64 sendBytes, qint64 size, bool atEnd)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    if (atEnd)
    {
        pLoadingScreen->hide();
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Downloaded record successfully."));
        m_pBaseMenu->addChild(pDialogMessageBox);
        exit();
    }
    else
    {
        qint32 progress = 0;
        if (size > 0)
        {
            progress = sendBytes * 100 / size;
        }
        pLoadingScreen->setProgress(tr("Downloading record from server ") + QString::number(sendBytes / 1024) + " kb / " + QString::number(size / 1024) + " kb", progress);
    }
}

QString DialogSelectDownloadRecord::getRecordPath(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        return recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_REPLAYFILE).toString();
    }
    else
    {
        return "";
    }
}

QString DialogSelectDownloadRecord::getMapName(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        return recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_MAPNAME).toString();
    }
    else
    {
        return "";
    }
}

QString DialogSelectDownloadRecord::getMapAuthor(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        return recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_MAPAUTHOR).toString();
    }
    else
    {
        return "";
    }
}

QString DialogSelectDownloadRecord::getRecordHeader(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        QJsonDocument doc(recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_REPLAYRECORDHEADER).toObject());
        return doc.toJson(QJsonDocument::Compact);
    }
    else
    {
        return "";
    }
}

qint32 DialogSelectDownloadRecord::getMapPlayers(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        return recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_MAPPLAYERS).toInt();
    }
    else
    {
        return 0;
    }
}

qint32 DialogSelectDownloadRecord::getMapWidth(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        return recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_MAPWIDTH).toInt();
    }
    else
    {
        return 0;
    }
}

qint32 DialogSelectDownloadRecord::getMapHeight(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        return recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_MAPHEIGHT).toInt();
    }
    else
    {
        return 0;
    }
}

qint64 DialogSelectDownloadRecord::getMapFlags(qint32 recordIndex)
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    if (recordIndex >= 0 && recordIndex < recordArray.size())
    {
        return recordArray[recordIndex].toObject().value(JsonKeys::JSONKEY_MAPFLAGS).toInteger();
    }
    else
    {
        return 0;
    }
}

qint32 DialogSelectDownloadRecord::getRecordCounts()
{
    QJsonArray recordArray = m_recordData.value(JsonKeys::JSONKEY_RECORDLIST).toArray();
    return recordArray.size();
}
