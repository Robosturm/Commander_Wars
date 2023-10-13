#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"

#include "objects/mapselection.h"

#include "coreengine/mainapp.h"
#include "coreengine/userdata.h"
#include "coreengine/globalutils.h"
#include "coreengine/gameconsole.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"

MapSelection::MapSelection(qint32 heigth, qint32 width, QString folder, const QStringList & filter)
    : m_filter(filter),
      m_currentFolder(Settings::getInstance()->getUserPath() + "maps"),
      m_itemChangedTimer(this)
{
#ifdef GRAPHICSUPPORT
    setObjectName("MapSelection");
#endif
    Interpreter::setCppOwnerShip(this);
    m_itemChangedTimer.setSingleShot(true);
    m_itemChangedTimer.setInterval(350);
    connect(&m_itemChangedTimer, &QTimer::timeout, this, &MapSelection::itemChangeTimerExpired, Qt::QueuedConnection);
    connect(this, &MapSelection::sigStartItemChangeTimer, this, &MapSelection::startItemChangeTimer, Qt::QueuedConnection);

    qint32 y = 5;

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    m_itemCount = 1;
    if (m_itemHeigth > 0)
    {
        m_itemCount = (heigth - 25) / m_itemHeigth;
    }
    createItemContainer(y, width, m_itemHeigth * m_itemCount);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("mapSelectionSelectedMap");
    m_SelectedItem = MemoryManagement::create<oxygine::Box9Sprite>();
    m_SelectedItem->setResAnim(pAnim);
    m_SelectedItem->setSize(m_ItemContainer->getContentWidth() - 60, m_itemHeigth);
    m_SelectedItem->setPosition(5, y);
    m_SelectedItem->setPriority(1);
    m_SelectedItem->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        if (!m_wasMoveScrolling)
        {
            m_itemClicked = true;
            emit itemClicked(m_currentItem);
        }
        else
        {
            m_wasMoveScrolling = false;
        }
    });
    changeFolder(folder);
    connect(this, &MapSelection::itemClicked, this, &MapSelection::changeFolder, Qt::QueuedConnection);
}

void MapSelection::createItemContainer(qint32 y, qint32 width, qint32 height)
{
    QSize size(width, height);
    m_ItemContainer = MemoryManagement::create<Panel>(true, size, size);
    m_ItemContainer->setPosition(0, y);
    addChild(m_ItemContainer);
}

void MapSelection::setSelection(QString folder, QStringList files)
{    
    CONSOLE_PRINT("MapSelection::setSelection", GameConsole::eDEBUG);
    m_itemClicked = false;
    m_currentFolder = folder;
    m_Files = files;
    updateSelection();
    if (m_currentIdx < m_Files.size() && m_currentIdx >= 0)
    {
        m_currentItem = m_Files[0];
        emit sigStartItemChangeTimer();
    }
}

void MapSelection::setCurrentItem(QString item)
{
    m_currentItem = item;
}

void MapSelection::refresh()
{
    changeFolder(m_currentFolder);
}

void MapSelection::changeFolder(QString folder)
{    
    CONSOLE_PRINT("MapSelection::changeFolder " + folder, GameConsole::eDEBUG);
    m_itemClicked = false;
    QString newFolder = folder;
    if (newFolder == "")
    {
        newFolder = Settings::getInstance()->getUserPath() + "maps";
    }
    else if (newFolder == "..")
    {
        newFolder = m_currentFolder + "/..";
    }
    while (newFolder.contains("//"))
    {
        newFolder = newFolder.replace("//", "/");
    }
    QDir dir(newFolder);
    QDir virtDir(oxygine::Resource::RCC_PREFIX_PATH + newFolder);
    if (dir.exists() || virtDir.exists())
    {
        QFileInfo newFolderInfo(newFolder);
        newFolder = GlobalUtils::makePathRelative(newFolderInfo.canonicalFilePath());
        CONSOLE_PRINT("MapSelection::changeFolder. Relative Path: " + newFolder, GameConsole::eDEBUG);
        m_Files.clear();
        if (newFolder != "maps")
        {
            m_Files.append("..");
        }
        QFileInfo upFolder(newFolder + "..");
        QStringList list;
        list.reserve(m_filter.size());
        for (const auto & ending : std::as_const(m_filter))
        {
            list.append("*" + ending);
        }
        QFileInfoList infoList = GlobalUtils::getInfoList(newFolder, list);
        Userdata* pUserdata = Userdata::getInstance();
        auto hideList = pUserdata->getShopItemsList(GameEnums::ShopItemType_Map, false);
        for (qint32 i = 1; i < infoList.size(); i++)
        {
            auto & infoItem = infoList[i];
            QString myPath = infoItem.canonicalFilePath();
            QString item = GlobalUtils::makePathRelative(myPath);
            bool isDir = infoItem.isDir();
            if ((item == newFolder) ||
                (upFolder == infoItem) ||
                (isDir && myPath.endsWith(".camp")) ||
                (hideList.contains(item)))
            {
                // skip ourself
                continue;
            }
            if (isDir)
            {
                QString path = infoItem.canonicalFilePath();
                QDirIterator iter(path, list, QDir::Files, QDirIterator::Subdirectories);
                QDirIterator iter2(oxygine::Resources::RCC_PREFIX_PATH + item, list, QDir::Files, QDirIterator::Subdirectories);
                if (!iter.hasNext() && !iter2.hasNext())
                {
                    continue;
                }
                m_Files.append(path);
            }
            else if (infoItem.isFile())
            {
                m_Files.append(infoItem.canonicalFilePath());
            }
        }
        m_currentFolder = newFolder;
        updateSelection();
        if (m_currentIdx < m_Files.size() && m_currentIdx >= 0)
        {
            m_currentItem = m_Files[m_currentIdx];
            emit sigStartItemChangeTimer();
        }
    }
}

void MapSelection::addNewSelectionItem(qint32 i, qint32 & y)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    const qint32 width = m_ItemContainer->getContentWidth() - 50;
    oxygine::ResAnim*  pAnim = pObjectManager->getResAnim("mapSelectionItem");
    oxygine::spBox9Sprite pBackground = MemoryManagement::create<oxygine::Box9Sprite>();
    pBackground->setResAnim(pAnim);
    pBackground->setSize(width, m_itemHeigth);
    pBackground->setPosition(0, y - m_ItemContainer->getY());
    spLabel pLabel = MemoryManagement::create<Label>(width - 20);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    pLabel->setStyle(style);
    pLabel->setHtmlText("");
    pLabel->setY(2);
    pLabel->setX(10);
    pBackground->addChild(pLabel);
    m_ItemContainer->addItem(pBackground);
    m_Items.append(pLabel);
    qint32 itemPos = m_Items.size() - 1;
    auto* pSelectedItem = m_SelectedItem.get();
    auto* pItem = m_Items[itemPos].get();
    auto* pItemContainer = m_ItemContainer.get();
    pBackground->addEventListener(oxygine::TouchEvent::OVER, [this, pSelectedItem, pItem, pItemContainer, i, y](oxygine::Event*)
                                  {
                                      if (pItem->getText() != "")
                                      {
                                          if (i < m_Files.size() && i >= 0)
                                          {
                                              pSelectedItem->setY(y - 5);
                                              m_currentItem = m_Files[i];
                                              m_currentIdx = i;
                                              emit sigStartItemChangeTimer();
                                          }
                                      }
                                  });
    pBackground->addEventListener(oxygine::TouchEvent::CLICK, [this, pSelectedItem, pItem, pItemContainer, i, y](oxygine::Event*)
                                  {
                                      if (pItem->getText() != "")
                                      {
                                          if (i < m_Files.size() &&  i >= 0)
                                          {
                                              pSelectedItem->setY(y - 5);
                                              m_currentItem = m_Files[i];
                                              m_currentIdx = i;
                                              emit sigStartItemChangeTimer();
                                          }
                                      }
                                  });
    y += m_itemHeigth;
}

void MapSelection::updateSelection()
{    
    m_itemClicked = false;
    m_ItemContainer->clearContent();
    m_Items.clear();
    m_ItemContainer->addItem(m_SelectedItem);
    qint32 y = 5;
    for (qint32 i = 0; i < m_Files.size() || i < m_itemCount; ++i)
    {
        addNewSelectionItem(i, y);
        if (i >= m_Files.size())
        {
            m_Items[i]->setHtmlText("");
        }
        else
        {
            QDir dir(m_Files[i]);
            if (m_Files[i] == "..")
            {
                m_Items[i]->setHtmlText(m_Files[i]);
            }
            else if (dir.exists())
            {
                QStringList data = m_Files[i].split("/");
                QStringList data2 = data[data.size() - 1].split(".");
                QStringList data3 = data2[0].split("_");
                QString item;
                for (qint32 i2 = 0; i2 < data3.size(); i2++)
                {
                    if (i2 > 0)
                    {
                        item += " ";
                    }
                    item += data3[i2].replace(0, 1, data3[i2][0].toUpper());
                }
                m_Items[i]->setHtmlText(item);
            }
            else
            {
                // it's a map
                QString fullFilename = m_Files[i];
                CONSOLE_PRINT("MapSelection::updateSelection Loading: " + fullFilename, GameConsole::eDEBUG);
                QFile file(fullFilename);
                file.open(QIODevice::ReadOnly);
                QDataStream pStream(&file);
                pStream.setVersion(QDataStream::Version::Qt_6_5);
                GameMap::MapHeaderInfo headerInfo;
                QString mapNameEnding = "";
                bool matches = true;
                if (m_Files[i].endsWith(".map"))
                {
                    GameMap::readMapHeader(pStream, headerInfo);
                    matches = m_mapFilter.matches(headerInfo);
                    mapNameEnding = " (" + QString::number(headerInfo.m_playerCount) + ")";
                }
                if (matches)
                {
                    if (headerInfo.m_mapName.isEmpty())
                    {
                        QStringList data = m_Files[i].split("/");
                        QStringList data2 = data[data.size() - 1].split(".");
                        QStringList data3 = data2[0].split("_");
                        QString item;
                        for (qint32 i2 = 0; i2 < data3.size(); i2++)
                        {
                            if (!data3[i2].isEmpty())
                            {
                                if (i2 > 0)
                                {
                                    item += " ";
                                }
                                item += data3[i2].replace(0, 1, data3[i2][0].toUpper());
                            }
                        }
                        m_Items[i]->setHtmlText(item + mapNameEnding);
                    }
                    else
                    {
                        m_Items[i]->setHtmlText(headerInfo.m_mapName + mapNameEnding);
                    }
                }
            }
        }
    }
    m_ItemContainer->setContentHeigth(y + 100);
    if (m_currentIdx + m_currentStartIndex< m_Files.size())
    {
        m_currentItem = m_Files[m_currentIdx + m_currentStartIndex];
        emit sigStartItemChangeTimer();
    }
    
}

void MapSelection::itemChangeTimerExpired()
{
    if (m_lastItem != m_currentItem)
    {
        m_lastItem = m_currentItem;
        emit itemChanged(m_currentItem);
        if (m_itemClicked)
        {
            m_itemClicked = false;
            emit itemClicked(m_currentItem);
        }
    }
}

void MapSelection::startItemChangeTimer()
{
    m_itemChangedTimer.start();
}

void MapSelection::filterChanged()
{
    updateSelection();
}
