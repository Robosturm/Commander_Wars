#include "coreengine/globalutils.h"
#include "coreengine/mainapp.h"

#include "multiplayer/lobbymenu.h"
#include "multiplayer/networkcommands.h"
#include "multiplayer/dialogselectdownloadmap.h"

#include "network/JsonKeys.h"

#include "objects/dialogs/mapSelection/mapselectionfilterdialog.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/spriteobject.h"

#include "ui_reader/uifactory.h"

static constexpr qint32 ITEMS_PER_PAGE = 20;

DialogSelectDownloadMap::DialogSelectDownloadMap(LobbyMenu* pBaseMenu)
    : CustomDialog("", "", pBaseMenu, tr("Close")),
    m_pBaseMenu(pBaseMenu)
{
    m_uiXml = "ui/multiplayer/selectDownloadMap.xml";
    loadXmlFile(m_uiXml);
    connect(m_pBaseMenu, &LobbyMenu::sigOnDownloadedResponse, this, &DialogSelectDownloadMap::onMapDownloaded, Qt::QueuedConnection);
    connect(m_pBaseMenu, &LobbyMenu::sigReceivedAvailableMaps, this, &DialogSelectDownloadMap::receivedMapData, Qt::QueuedConnection);
}

void DialogSelectDownloadMap::showMapFilter()
{
    spMapSelectionFilterDialog mapSelectionFilterDialog = MemoryManagement::create<MapSelectionFilterDialog>(&m_mapFilter);
    addChild(mapSelectionFilterDialog);
    connect(mapSelectionFilterDialog.get(), &MapSelectionFilterDialog::sigFinished, this, &DialogSelectDownloadMap::filterChanged, Qt::QueuedConnection);
}

qint32 DialogSelectDownloadMap::getCurrentPage() const
{
    return m_currentStartIndex % ITEMS_PER_PAGE;
}

void DialogSelectDownloadMap::changeCurrentPageToEnd(bool start)
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

void DialogSelectDownloadMap::changeCurrentPage(qint32 direction)
{
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
    qint32 pageCount = getPageCount();
    if (m_currentStartIndex >= pageCount)
    {
        m_currentStartIndex = pageCount - 1;
    }
    filterChanged();
}

qint32 DialogSelectDownloadMap::getPageCount()
{
    qint32 items = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toInt();
    qint32 page = items / ITEMS_PER_PAGE;
    if (page % ITEMS_PER_PAGE > 0)
    {
        ++page;
    }
    return page;
}

void DialogSelectDownloadMap::filterChanged()
{
    QString command = QString(NetworkCommands::REQUESTAVAILABLEMAPS);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data = m_mapFilter.toJson();
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_ITEMSPERPAGE, ITEMS_PER_PAGE);
    data.insert(JsonKeys::JSONKEY_STARTINDEX, m_currentStartIndex);
    m_pBaseMenu->requestAvailableMaps(data);
}

void DialogSelectDownloadMap::receivedMapData(const QJsonObject &objData)
{
    m_mapData = objData;
    m_currentStartIndex = objData.value(JsonKeys::JSONKEY_STARTINDEX).toInt();
    m_minimapImages.clear();
    m_minimapSprites.clear();
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST ).toArray();
    for (const auto & mapData : mapArray)
    {
        QByteArray data = GlobalUtils::toByteArray(mapData.toObject().value(JsonKeys::JSONKEY_MINIMAPDATA).toArray());
        QImage img;
        QBuffer buffer(&data);
        buffer.open(QIODevice::ReadOnly);
        img.load(&buffer, "PNG");
        m_minimapImages.push_back(img);
        oxygine::spSingleResAnim minimapImage = MemoryManagement::create<oxygine::SingleResAnim>();
        Mainapp::getInstance()->loadResAnim(minimapImage, img, 1, 1, 1);
        m_minimapSprites.push_back(minimapImage);
    }
    CONSOLE_PRINT("Refreshing ui with the received maps " + QString::number(m_minimapImages.size()), GameConsole::eDEBUG);
    refreshUi();
}

oxygine::spActor DialogSelectDownloadMap::loadCustomId(const QString & item, qint32 x, qint32 y, bool enabled, bool visible, float scale,
                                                       const QString & id, const QString & tooltip, const QString & onEvent,
                                                       UiFactory* pFactoty, CreatedGui* pMenu, qint32 loopIdx, qint32 & scaledWidth, qint32 & scaledHeight)
{
    if (item == "MINIMAP")
    {
        spSpriteObject pSprite = MemoryManagement::create<SpriteObject>();
        pSprite->setObjectName(id);
        pSprite->setResAnim(m_minimapSprites[loopIdx].get());
        pSprite->setScale(scale);
        pSprite->setPosition(x, y);
        pSprite->setVisible(visible);
        pSprite->setEnabled(enabled);
        pSprite->setSize(m_minimapSprites[loopIdx]->getSize() * scale);
        scaledWidth = m_minimapSprites[loopIdx]->getWidth() * scale;
        scaledHeight = m_minimapSprites[loopIdx]->getHeight() * scale;
        if (!onEvent.isEmpty())
        {
            pSprite->addClickListener([pFactoty, onEvent, id, loopIdx, pMenu](oxygine::Event*)
                                      {
                                          emit pFactoty->sigDoEvent(onEvent, id, loopIdx, pMenu);
                                      });
        }
        return pSprite;
    }
    else
    {
        return oxygine::spActor();
    }
}

qint32 DialogSelectDownloadMap::getMapImageHeight(qint32 mapIndex)
{
    if (mapIndex >= 0 && mapIndex < m_minimapSprites.size())
    {
        return m_minimapSprites[mapIndex]->getHeight();
    }
    else
    {
        return 0;
    }
}

void DialogSelectDownloadMap::downloadMap(qint32 mapIndex)
{
    QString command = QString(NetworkCommands::FILEDOWNLOADREQUEST);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_MAPPATH, getMapPath(mapIndex));
    m_pBaseMenu->requestDownloadMap(data);
}

void DialogSelectDownloadMap::onMapDownloaded(bool success)
{
    if (success)
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Downloaded map successfully."));
        m_pBaseMenu->addChild(pDialogMessageBox);
        exit();
    }
    else
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Failed to download map from the server."));
        addChild(pDialogMessageBox);
    }
}

QString DialogSelectDownloadMap::getMapPath(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST).toArray();
    if (mapIndex >= 0 && mapIndex < mapArray.size())
    {
        return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPPATH).toString();
    }
    else
    {
        return "";
    }
}

QString DialogSelectDownloadMap::getMapName(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST).toArray();
    if (mapIndex >= 0 && mapIndex < mapArray.size())
    {
        return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPNAME).toString();
    }
    else
    {
        return "";
    }
}

QString DialogSelectDownloadMap::getMapAuthor(qint32 mapIndex)
{
    if (mapIndex >= 0 && mapIndex < m_minimapSprites.size())
    {
        QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST).toArray();
        return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPAUTHOR).toString();
    }
    else
    {
        return "";
    }
}

qint32 DialogSelectDownloadMap::getMapPlayers(qint32 mapIndex)
{
    if (mapIndex >= 0 && mapIndex < m_minimapSprites.size())
    {
        QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST).toArray();
        return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPPLAYERS).toInt();
    }
    else
    {
        return 0;
    }
}

qint32 DialogSelectDownloadMap::getMapWidth(qint32 mapIndex)
{
    if (mapIndex >= 0 && mapIndex < m_minimapSprites.size())
    {
        QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST).toArray();
        return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPWIDTH).toInt();
    }
    else
    {
        return 0;
    }
}

qint32 DialogSelectDownloadMap::getMapHeight(qint32 mapIndex)
{
    if (mapIndex >= 0 && mapIndex < m_minimapSprites.size())
    {
        QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST).toArray();
        return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPHEIGHT).toInt();
    }
    else
    {
        return 0;
    }
}

qint64 DialogSelectDownloadMap::getMapFlags(qint32 mapIndex)
{
    if (mapIndex >= 0 && mapIndex < m_minimapSprites.size())
    {
        QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_MAPLIST).toArray();
        return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPFLAGS).toInteger();
    }
    else
    {
        return 0;
    }
}

qint32 DialogSelectDownloadMap::getMapCounts()
{
    return m_minimapSprites.size();
}

