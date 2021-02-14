#include "qfile.h"

#include "coreengine/userdata.h"
#include "coreengine/settings.h"
#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"
#include "coreengine/filesupport.h"

#include "resource_management/cospritemanager.h"

#include "objects/achievementbanner.h"

Userdata* Userdata::m_pInstance = nullptr;

Userdata* Userdata::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new Userdata();
    }
    return m_pInstance;
}

Userdata::Userdata()
    : QObject()
{
    Interpreter::setCppOwnerShip(this);
    changeUser();
}

qint32 Userdata::getCredtis() const
{
    return m_credtis;
}

void Userdata::setCredtis(const qint32 &credtis)
{
    m_credtis = credtis;
}

void Userdata::addCredtis(const qint32 &credtis)
{
    m_credtis += credtis;
}

void Userdata::storeUser()
{
    Mainapp* pApp = Mainapp::getInstance();
    if (!pApp->getSlave())
    {
        if (!Settings::getUsername().isEmpty())
        {
            QFile user(Settings::getUsername() + ".dat");
            user.remove();
            user.open(QIODevice::WriteOnly);
            QDataStream pStream(&user);
            serializeObject(pStream);
            user.close();
        }
    }
}

void Userdata::changeUser()
{
    m_customCOStyles.clear();
    QFile user(Settings::getUsername() + ".dat");
    if (user.exists())
    {
        user.open(QIODevice::ReadOnly);
        QDataStream pStream(&user);
        deserializeObject(pStream);
        user.close();
    }
}

void Userdata::addCOStyle(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox)
{
    for (qint32 i = 0; i < m_customCOStyles.size(); i++)
    {
        if (coid == std::get<0>(m_customCOStyles[i]))
        {            
            colorTable.convertTo(QImage::Format_ARGB32);
            maskTable.convertTo(QImage::Format_ARGB32);
            std::get<1>(m_customCOStyles[i]) = file;
            std::get<2>(m_customCOStyles[i]) = colorTable;
            std::get<3>(m_customCOStyles[i]) = maskTable;
            std::get<4>(m_customCOStyles[i]) = useColorBox;
            storeUser();
            return;
        }
    }
    m_customCOStyles.append(std::tuple<QString, QString, QImage, QImage, bool>(coid, file, colorTable, maskTable, useColorBox));
    storeUser();
}

void Userdata::removeCOStyle(QString coid)
{
    for (qint32 i = 0; i < m_customCOStyles.size(); i++)
    {
        if (coid == std::get<0>(m_customCOStyles[i]))
        {
            m_customCOStyles.removeAt(i);
            break;
        }
    }
}

std::tuple<QString, QString, QImage, QImage, bool>* Userdata::getCOStyle(QString coid)
{
    for (qint32 i = 0; i < m_customCOStyles.size(); i++)
    {
        if (coid == std::get<0>(m_customCOStyles[i]))
        {
            return &m_customCOStyles[i];
        }
    }
    return nullptr;
}

void Userdata::increaseAchievement(QString id, qint32 value)
{
    for (auto & achievement : m_achievements)
    {
        if (achievement.id == id)
        {
            bool achieved = (achievement.progress >= achievement.targetValue);
            achievement.progress += value;
            if (!achieved && (achievement.progress >= achievement.targetValue))
            {
                spAchievementBanner banner = new AchievementBanner(achievement);
                oxygine::getStage()->addChild(banner);
                
            }
        }
    }
}

void Userdata::deleteAchievement(QString id)
{
    for (qint32 i = 0; i < m_achievements.size(); i++)
    {
        if (m_achievements[i].id == id)
        {
            m_achievements.removeAt(i);
            break;
        }
    }
}

bool Userdata::achieved(QString id)
{
    for (const auto & achievement : m_achievements)
    {
        if (achievement.id == id)
        {
           return (achievement.progress >= achievement.targetValue);
        }
    }
    return false;
}

QString Userdata::getActiveCoStyle(QString coid)
{
    for (const auto & style : m_customCOStyles)
    {
        if (coid == std::get<0>(style))
        {
            return std::get<1>(style);
        }
    }
    return "";
}

void Userdata::addAchievement(QString id, qint32 targetValue, QString name, QString description, QString icon, bool hide)
{
    bool found = false;
    for (auto & achievement : m_achievements)
    {
        if (achievement.id == id)
        {
            found = true;
            achievement.targetValue = targetValue;
            achievement.name = name;
            achievement.description = description;
            achievement.hide = hide;
            achievement.icon = icon;
            achievement.loaded = true;
            break;
        }
    }
    if (!found)
    {
        Achievement achievement;
        achievement.id = id;
        achievement.targetValue = targetValue;
        achievement.name = name;
        achievement.description = description;
        achievement.hide = hide;
        achievement.icon = icon;
        achievement.loaded = true;
        m_achievements.append(achievement);
    }
}

QVector<Userdata::Achievement>* Userdata::getAchievements()
{
    return &m_achievements;
}

void Userdata::addVictoryForMap(QString mapPath, QString co1, QString co2, qint32 score)
{
    if (m_mapVictoryInfo.contains(mapPath))
    {
        auto item = m_mapVictoryInfo.find(mapPath);
        qint32 insertPos = -1;
        if (item->co1.size() >= MAX_VICTORY_INFO_PER_MAP)
        {
            for (qint32 i2 = 0; i2 < MAX_VICTORY_INFO_PER_MAP; ++i2)
            {
                if (item->score[i2] <= score)
                {
                    insertPos = i2;
                    item->co1.removeLast();
                    item->co2.removeLast();
                    item->score.removeLast();
                    break;
                }
            }
        }
        else
        {
            insertPos = item->score.length();
            for (qint32 i2 = 0; i2 < item->score.length(); ++i2)
            {
                if (item->score[i2] <= score)
                {
                    insertPos = i2;
                }
            }
        }
        if (insertPos >= 0)
        {
            item->co1.insert(insertPos, co1);
            item->co2.insert(insertPos, co2);
            item->score.insert(insertPos, score);
        }
    }
    else
    {
        MapVictoryInfo info;
        info.co1.append(co1);
        info.co2.append(co2);
        info.score.append(score);
        m_mapVictoryInfo.insert(mapPath, info);
    }
}

const Userdata::MapVictoryInfo * Userdata::getVictoryForMap(QString mapPath)
{
    if (m_mapVictoryInfo.contains(mapPath))
    {
        return &m_mapVictoryInfo[mapPath];
    }
    return nullptr;
}

QVector<Userdata::ShopItem> Userdata::getItems(GameEnums::ShopItemType type, bool buyable, bool bought)
{
    QVector<Userdata::ShopItem> ret;
    for (const auto & item : m_shopItems)
    {
        if ((item.itemType == type || type == GameEnums::ShopItemType::ShopItemType_All) &&
            item.buyable == buyable &&
            item.bought == bought)
        {
            ret.append(item);
        }
    }
    return ret;
}

QVector<Userdata::ShopItem> Userdata::getItems(GameEnums::ShopItemType type, bool bought)
{
    QVector<Userdata::ShopItem> ret;
    for (const auto & item : m_shopItems)
    {
        if ((item.itemType == type || type == GameEnums::ShopItemType::ShopItemType_All) &&
            item.bought == bought)
        {
            ret.append(item);
        }
    }
    return ret;
}

QStringList Userdata::getItemsList(GameEnums::ShopItemType type, bool bought)
{
    QStringList ret;
    for (const auto & item : m_shopItems)
    {
        if ((item.itemType == type || type == GameEnums::ShopItemType::ShopItemType_All) &&
            item.bought == bought)
        {
            ret.append(item.key);
        }
    }
    return ret;
}

void Userdata::addShopItem(GameEnums::ShopItemType itemType, QString key, QString name, qint32 price, bool buyable)
{
    bool found = false;
    for (const auto & item : m_shopItems)
    {
        if (item.itemType == itemType &&
            item.key == key)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        ShopItem item;
        item.key = key;
        item.name = name;
        item.price = price;
        item.buyable = buyable;
        item.bought = false;
        item.itemType = itemType;
        m_shopItems.append(item);
    }
}

void Userdata::setShopItemBuyable(GameEnums::ShopItemType itemType, QString key, bool buyable)
{
    for (auto & item : m_shopItems)
    {
        if (item.itemType == itemType &&
            item.key == key)
        {
            item.buyable = buyable;
            break;
        }
    }
}

void Userdata::setShopItemBought(GameEnums::ShopItemType itemType, QString key, bool bought)
{
    for (auto & item : m_shopItems)
    {
        if (item.itemType == itemType &&
            item.key == key)
        {
            item.bought = bought;
            break;
        }
    }
}

void Userdata::unlockAllShopItems(bool bought)
{
    for (auto & item : m_shopItems)
    {
        item.buyable = true;
        item.bought |= bought;
    }
}

void Userdata::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_customCOStyles.size());
    for (qint32 i = 0; i < m_customCOStyles.size(); i++)
    {
        pStream << std::get<0>(m_customCOStyles[i]);
        pStream << std::get<1>(m_customCOStyles[i]);
        qint32 width = static_cast<qint32>(std::get<2>(m_customCOStyles[i]).width());
        pStream << width;
        for (qint32 x = 0; x < width; x++)
        {
            pStream << std::get<2>(m_customCOStyles[i]).pixel(x, 0);
            pStream << std::get<3>(m_customCOStyles[i]).pixel(x, 0);
        }
        pStream << std::get<4>(m_customCOStyles[i]);
    }
    pStream << static_cast<qint32>(m_achievements.size());
    for (qint32 i = 0; i < m_achievements.size(); i++)
    {
        pStream << m_achievements[i].id;
        pStream << m_achievements[i].progress;
    }
    pStream << static_cast<qint32>(m_mapVictoryInfo.size());
    auto keys = m_mapVictoryInfo.keys();
    for (auto key : keys)
    {
        auto & item = m_mapVictoryInfo[key];
        pStream << key;
        Filesupport::writeVectorList(pStream, item.co1);
        Filesupport::writeVectorList(pStream, item.co2);
        Filesupport::writeVectorList(pStream, item.score);
    }
    pStream << static_cast<qint32>(m_shopItems.size());
    for (auto & item : m_shopItems)
    {
        pStream << item.key;
        pStream << item.name;
        pStream << item.price;
        pStream << item.buyable;
        pStream << item.bought;
        pStream << static_cast<qint32>(item.itemType);
    }
    pStream << m_credtis;
}

void Userdata::deserializeObject(QDataStream& pStream)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    m_customCOStyles.clear();
    for (qint32 i = 0; i < size; i++)
    {
        QString coid;
        QString file;
        QImage colorTable;
        QImage maskTable;
        bool useColorBox = false;
        pStream >> coid;
        pStream >> file;
        if (version > 2)
        {
            qint32 width = 0;
            pStream >> width;
            colorTable = QImage(width, 1, QImage::Format_ARGB32);
            maskTable = QImage(width, 1, QImage::Format_ARGB32);
            QRgb rgb;
            for (qint32 x = 0; x < width; x++)
            {
                pStream >> rgb;
                colorTable.setPixel(x, 0, rgb);
                pStream >> rgb;
                maskTable.setPixel(x, 0, rgb);
            }
        }
        else
        {
            pStream >> colorTable;
            pStream >> maskTable;
        }
        if (version > 1)
        {
            pStream >> useColorBox;
        }
        colorTable.convertTo(QImage::Format_ARGB32);
        maskTable.convertTo(QImage::Format_ARGB32);
        pCOSpriteManager->loadResAnim(coid, file, colorTable, maskTable, useColorBox);
        m_customCOStyles.append(std::tuple<QString, QString, QImage, QImage, bool>(coid, file, colorTable, maskTable, useColorBox));
    }
    if (version > 3)
    {
        pStream >> size;
        m_achievements.clear();
        for (qint32 i = 0; i < size; i++)
        {
            m_achievements.append(Achievement());
            pStream >> m_achievements[i].id;
            pStream >> m_achievements[i].progress;
        }
    }
    if (version > 4)
    {
        pStream >> size;
        m_mapVictoryInfo.clear();
        for (qint32 i = 0; i < size; i++)
        {
            QString key;
            MapVictoryInfo item;
            pStream >> key;
            item.co1 = Filesupport::readVectorList<QString, QList>(pStream);
            item.co2 = Filesupport::readVectorList<QString, QList>(pStream);
            item.score = Filesupport::readVectorList<qint32, QVector>(pStream);
            m_mapVictoryInfo.insert(key, item);
        }
        pStream >> size;
        m_shopItems.clear();
        for (qint32 i = 0; i < size; i++)
        {
            QString key;
            ShopItem item;
            pStream >> item.key;
            pStream >> item.name;
            pStream >> item.price;
            pStream >> item.buyable;
            pStream >> item.bought;
            if (version > 5)
            {
                qint32 value = 0;
                pStream >> value;
                item.itemType = static_cast<GameEnums::ShopItemType>(value);
            }
            m_shopItems.append(item);
        }
    }
    if (version > 5)
    {
        pStream >> m_credtis;
        // bug fix for not initialised variable --> not sure if enabled in the last release or not
        if (m_credtis < 0)
        {
            m_credtis = 0;
        }
    }
}
