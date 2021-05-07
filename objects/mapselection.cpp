#include "mapselection.h"

#include "coreengine/mainapp.h"
#include "coreengine/userdata.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"

#include "QDir"

MapSelection::MapSelection(qint32 heigth, qint32 width, QString folder)
    : QObject(),
      m_currentFolder(QCoreApplication::applicationDirPath() + "/maps/")
{
    setObjectName("MapSelection");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    m_itemChangedTimer.setSingleShot(true);
    m_itemChangedTimer.setInterval(350);
    connect(&m_itemChangedTimer, &QTimer::timeout, this, &MapSelection::itemChangeTimerExpired, Qt::QueuedConnection);
    connect(this, &MapSelection::sigStartItemChangeTimer, this, &MapSelection::startItemChangeTimer, Qt::QueuedConnection);

    oxygine::spButton pArrowUp = oxygine::spButton::create();
    oxygine::ResAnim* pAnim = ObjectManager::getInstance()->getResAnim("arrow+down");
    pArrowUp->setResAnim(pAnim);
    pArrowUp->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    pArrowUp->setFlippedY(true);
    pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        m_spin = -1;
        m_timer.start();
        emit changeSelection(m_currentStartIndex - 1);
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_spin = 0;
    });
    pArrowUp->setScaleX(4.0f);
    pArrowUp->setPosition(width / 2 - pAnim->getWidth() * 2, 0);
    this->addChild(pArrowUp);

    qint32 arrowHeigth = static_cast<qint32>(pAnim->getHeight());
    qint32 y = arrowHeigth + 5;


    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    pAnim = pObjectManager->getResAnim("mapSelectionTop");
    oxygine::spBox9Sprite pBackground = oxygine::spBox9Sprite::create();
    pBackground->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBackground->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBackground->setResAnim(pAnim);
    pBackground->setSize(width, pAnim->getHeight());
    pBackground->setPosition(0, y);
    y += pAnim->getHeight();
    this->addChild(pBackground);

     pAnim = pObjectManager->getResAnim("mapSelectionSelectedMap");
    m_SelectedItem = oxygine::spBox9Sprite::create();
    m_SelectedItem->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_SelectedItem->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_SelectedItem->setResAnim(pAnim);
    m_SelectedItem->setSize(width - 12, m_itemHeigth);
    m_SelectedItem->setPosition(5, y);
    m_SelectedItem->setPriority(1);
    this->addChild(m_SelectedItem);
    m_SelectedItem->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        this->m_itemClicked = true;
        emit itemClicked(m_currentItem);
    });

    m_itemCount = (heigth - 10 - 2 * static_cast<qint32>(pAnim->getHeight()) - arrowHeigth * 2) / m_itemHeigth;
    for (qint32 i = 0; i < m_itemCount; i++)
    {
        pAnim = pObjectManager->getResAnim("mapSelectionItem");
        pBackground = oxygine::spBox9Sprite::create();
        pBackground->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pBackground->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pBackground->setResAnim(pAnim);
        pBackground->setSize(width, m_itemHeigth);
        pBackground->setPosition(0, y);
        oxygine::spClipRectActor pClipActor = oxygine::spClipRectActor::create();
        pBackground->addChild(pClipActor);
        oxygine::spTextField pTextfield = oxygine::spTextField::create();
        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        pTextfield->setStyle(style);
        pTextfield->setHtmlText("");
        pTextfield->setY(5);
        pClipActor->addChild(pTextfield);
        pClipActor->setX(10);
        pClipActor->setSize(width - 20, m_itemHeigth);
        this->addChild(pBackground);
        m_Items.append(pTextfield);
        qint32 itemPos = m_Items.size() - 1;
        pBackground->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
        {
            if (m_Items[itemPos]->getText() != "")
            {
                m_SelectedItem->setY(y);
                m_currentItem = m_Files[m_currentStartIndex + i];
                m_currentIdx = m_currentStartIndex + i;
                emit sigStartItemChangeTimer();
            }
        });

        y += m_itemHeigth;
    }
    pAnim = pObjectManager->getResAnim("mapSelectionBottom");
    pBackground = oxygine::spBox9Sprite::create();
    pBackground->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBackground->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBackground->setResAnim(pAnim);
    pBackground->setSize(width, pAnim->getHeight());
    pBackground->setPosition(0, y);
    y += pAnim->getHeight();
    this->addChild(pBackground);

    oxygine::spButton pArrowDown = oxygine::spButton::create();
    pAnim = ObjectManager::getInstance()->getResAnim("arrow+down");
    pArrowDown->setResAnim(pAnim);
    pArrowDown->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        m_spin = +1;
        m_timer.start();
        emit changeSelection(m_currentStartIndex + 1);
    });
    pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_spin = 0;
    });
    pArrowDown->setScaleX(4.0f);
    pArrowDown->setPosition(width / 2 - pAnim->getWidth() * 2, y + 5);
    this->addChild(pArrowDown);
    changeFolder(folder);
    connect(this, &MapSelection::changeSelection, this, &MapSelection::updateSelection, Qt::QueuedConnection);
    connect(this, &MapSelection::itemClicked, this, &MapSelection::changeFolder, Qt::QueuedConnection);
    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [ = ](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
           emit changeSelection(static_cast<qint32>(-pTouchEvent->wheelDirection.y / 100) + m_currentStartIndex);
           pTouchEvent->stopPropagation();
        }
    });
}

MapSelection::~MapSelection()
{

}

void MapSelection::setSelection(QString folder, QStringList files)
{    
    m_itemClicked = false;
    m_currentFolder = folder;
    m_Files = files;
    updateSelection(0);
    if (m_Files.size() > 0)
    {
        m_currentItem = m_Files[0];
        emit sigStartItemChangeTimer();
    }    
}

void MapSelection::setCurrentItem(QString item)
{
    m_currentItem = item;
}

void MapSelection::changeFolder(QString folder)
{    
    m_itemClicked = false;
    QString newFolder = folder;
    if (folder == "")
    {
        newFolder = QCoreApplication::applicationDirPath() + "/maps/";
    }
    if (folder == "..")
    {
        newFolder = m_currentFolder + "/..";
    }
    QDir dir(newFolder);
    if (dir.exists())
    {
        QFileInfo newFolderInfo(newFolder);
        newFolder = newFolderInfo.absoluteFilePath() + "/";
        m_Files.clear();
        if ((newFolder != QCoreApplication::applicationDirPath() + "/maps//") &&
            (newFolder != QCoreApplication::applicationDirPath() + "/maps/"))
        {
            m_Files.append("..");
        }
        QFileInfoList infoList;
        QFileInfo upFolder(newFolder + "..");
        QString list = "*.map;*.jsm";
        infoList.append(QDir(newFolder).entryInfoList(QDir::Dirs));
        infoList.append(QDir(newFolder).entryInfoList(list.split(";"), QDir::Files));
        Userdata* pUserdata = Userdata::getInstance();
        auto hideList = pUserdata->getShopItemsList(GameEnums::ShopItemType_Map, false);
        for (qint32 i = 1; i < infoList.size(); i++)
        {
            QString myPath = infoList[i].absoluteFilePath();
            QString item = myPath;
            item.replace(QCoreApplication::applicationDirPath() + "/", "");
            if ((myPath == newFolder) ||
                (upFolder == infoList[i] ||
                 (infoList[i].isDir() && myPath.endsWith(".camp"))) ||
                 (hideList.contains(item)))
            {
                // skip ourself
                continue;
            }

            if (infoList[i].isDir())
            {
                QString path = infoList[i].absoluteFilePath();
                m_Files.append(path);
            }
            else if (infoList[i].isFile())
            {
                m_Files.append(infoList[i].fileName());
            }
        }
        m_currentFolder = newFolder;
        updateSelection(0);
        if (m_currentIdx < m_Files.size())
        {
            m_currentItem = m_Files[m_currentIdx];
            emit sigStartItemChangeTimer();
        }
    }
    
}

void MapSelection::update(const oxygine::UpdateState& us)
{
    if (m_spin != 0 && m_timer.elapsed() >= 250)
    {
        m_timer.start();
        emit changeSelection(m_currentStartIndex + m_spin);
    }

    oxygine::Actor::update(us);
}

void MapSelection::updateSelection(qint32 startIndex)
{
    
    m_itemClicked = false;
    m_currentStartIndex = startIndex;
    if (m_currentStartIndex < 0)
    {
        m_currentStartIndex = 0;
    }
    else if (m_currentStartIndex >= m_Files.size() - m_itemCount)
    {
        m_currentStartIndex = m_Files.size() - m_itemCount;
        if (m_currentStartIndex < 0)
        {
            m_currentStartIndex = 0;
        }
    }
    for (qint32 i = 0; i < m_itemCount; i++)
    {
        if (m_currentStartIndex + i >= m_Files.size())
        {
            m_Items[i]->setHtmlText("");
        }
        else
        {
            QDir dir(m_Files[m_currentStartIndex + i]);
            if (m_Files[m_currentStartIndex + i] == "..")
            {
                m_Items[i]->setHtmlText(m_Files[m_currentStartIndex + i]);
            }
            else if (dir.exists())
            {
                QStringList data = m_Files[m_currentStartIndex + i].split("/");
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
                QFile file(m_currentFolder + m_Files[m_currentStartIndex + i]);
                file.open(QIODevice::ReadOnly);
                QDataStream pStream(&file);
                QString name;
                qint32 version = 0;
                QString mapAuthor;
                QString mapDescription;
                qint32 width = 0;
                qint32 heigth = 0;
                qint32 playerCount = 0;
                qint32 uniqueIdCounter = 0;
                QString mapNameEnding = "";
                if (m_Files[m_currentStartIndex + i].endsWith(".map"))
                {
                    GameMap::readMapHeader(pStream, version, name, mapAuthor, mapDescription,
                                           width, heigth, playerCount, uniqueIdCounter);
                    mapNameEnding = " (" + QString::number(playerCount) + ")";
                }
                if (name.isEmpty())
                {
                    QStringList data = m_Files[m_currentStartIndex + i].split("/");
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
                    m_Items[i]->setHtmlText(item + mapNameEnding);
                }
                else
                {
                    m_Items[i]->setHtmlText(name + mapNameEnding);
                }
            }
        }
    }
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
