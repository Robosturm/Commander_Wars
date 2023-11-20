#include "coreengine/globalutils.h"
#include "coreengine/mainapp.h"

#include "multiplayer/lobbymenu.h"
#include "multiplayer/networkcommands.h"
#include "multiplayer/dialogselectdownloadmap.h"

#include "network/JsonKeys.h"

#include "objects/dialogs/mapSelection/mapselectionfilterdialog.h"
#include "objects/base/spriteobject.h"

#include "ui_reader/uifactory.h"

DialogSelectDownloadMap::DialogSelectDownloadMap(LobbyMenu* pBaseMenu)
    : CustomDialog("", "", pBaseMenu, tr("Close")),
    m_pBaseMenu(pBaseMenu)
{
    m_uiXml = "ui/multiplayer/selectDownloadMap.xml";
    loadXmlFile(m_uiXml);
    connect(m_pBaseMenu, &LobbyMenu::sigOnDownloadedResponse, this, &DialogSelectDownloadMap::onMapDownloaded, Qt::QueuedConnection);
}

void DialogSelectDownloadMap::showMapFilter()
{
    spMapSelectionFilterDialog mapSelectionFilterDialog = MemoryManagement::create<MapSelectionFilterDialog>(&m_mapFilter);
    addChild(mapSelectionFilterDialog);
    connect(mapSelectionFilterDialog.get(), &MapSelectionFilterDialog::sigFinished, this, &DialogSelectDownloadMap::filterChanged, Qt::QueuedConnection);
}

qint32 DialogSelectDownloadMap::getCurrentPage() const
{
    return m_currentPage;
}

void DialogSelectDownloadMap::setCurrentPage(qint32 newCurrentPage)
{
    m_currentPage = newCurrentPage;
}

void DialogSelectDownloadMap::filterChanged()
{
    QString command = QString(NetworkCommands::REQUESTAVAILABLEMAPS);
    QJsonObject data = m_mapFilter.toJson();
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    m_pBaseMenu->requestAvailableMaps(data);
}

void DialogSelectDownloadMap::receivedMapData(const QJsonObject &objData)
{
    m_mapData = objData;
    m_minimapImages.clear();
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
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

void DialogSelectDownloadMap::downloadMap(qint32 mapIndex)
{
    QString command = QString(NetworkCommands::FILEDOWNLOAD);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_MAPPATH, getMapPath(mapIndex));
    m_pBaseMenu->requestDownloadMap(data);
}

void DialogSelectDownloadMap::onMapDownloaded(bool success)
{
    if (success)
    {
        exit();
    }
    else
    {

    }
}

QString DialogSelectDownloadMap::getMapPath(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
    return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPPATH).toString();
}

QString DialogSelectDownloadMap::getMapName(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
    return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPNAME).toString();
}

QString DialogSelectDownloadMap::getMapAuthor(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
    return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPAUTHOR).toString();
}

qint32 DialogSelectDownloadMap::getMapPlayers(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
    return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPPLAYERS).toInt();
}

qint32 DialogSelectDownloadMap::getMapWidth(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
    return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPWIDTH).toInt();
}

qint32 DialogSelectDownloadMap::getMapHeight(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
    return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPHEIGHT).toInt();
}

qint64 DialogSelectDownloadMap::getMapFlags(qint32 mapIndex)
{
    QJsonArray mapArray = m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray();
    return mapArray[mapIndex].toObject().value(JsonKeys::JSONKEY_MAPFLAGS).toInteger();
}

qint32 DialogSelectDownloadMap::getMapCounts()
{
    return m_mapData.value(JsonKeys::JSONKEY_FOUNDITEMS).toArray().size();
}

