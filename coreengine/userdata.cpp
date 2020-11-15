#include "userdata.h"

#include "resource_management/cospritemanager.h"

#include "qfile.h"

#include "coreengine/settings.h"
#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"
#include "coreengine/filesupport.h"
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
    bool found = false;
    for (qint32 i = 0; i < m_mapVictoryInfo.size(); i++)
    {
        if (mapPath == m_mapVictoryInfo[i].mapPath)
        {
            qint32 insertPos = -1;
            if (m_mapVictoryInfo[i].co1.size() >= MAX_VICTORY_INFO_PER_MAP)
            {
                for (qint32 i2 = 0; i2 < score; ++i2)
                {
                    if (m_mapVictoryInfo[i].score[i2] <= score)
                    {
                        insertPos = i2;
                        m_mapVictoryInfo[i].co1.removeLast();
                        m_mapVictoryInfo[i].co2.removeLast();
                        m_mapVictoryInfo[i].score.removeLast();
                        break;
                    }
                }
            }
            else
            {
                insertPos = m_mapVictoryInfo[i].score.length();
                for (qint32 i2 = 0; i2 < m_mapVictoryInfo[i].score.length(); ++i2)
                {
                    if (m_mapVictoryInfo[i].score[i2] <= score)
                    {
                        insertPos = i2;
                        break;
                    }
                }
            }
            if (insertPos >= 0)
            {
                m_mapVictoryInfo[i].co1.insert(insertPos, co1);
                m_mapVictoryInfo[i].co2.insert(insertPos, co2);
                m_mapVictoryInfo[i].score.insert(insertPos, score);
            }
            found = true;
        }
    }
    if (!found)
    {
        qint32 i = m_mapVictoryInfo.length();
        m_mapVictoryInfo.append(MapVictoryInfo());
        m_mapVictoryInfo[i].mapPath = mapPath;
        m_mapVictoryInfo[i].co1.append(co1);
        m_mapVictoryInfo[i].co2.append(co2);
        m_mapVictoryInfo[i].score.append(score);
    }
}

const Userdata::MapVictoryInfo * Userdata::getVictoryForMap(QString mapPath)
{
    for (qint32 i = 0; i < m_mapVictoryInfo.size(); i++)
    {
        if (mapPath == m_mapVictoryInfo[i].mapPath)
        {
            return &m_mapVictoryInfo[i];
        }
    }
    return nullptr;
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
    for (qint32 i = 0; i < m_mapVictoryInfo.size(); i++)
    {
        pStream << m_mapVictoryInfo[i].mapPath;
        Filesupport::writeVectorList(pStream, m_mapVictoryInfo[i].co1);
        Filesupport::writeVectorList(pStream, m_mapVictoryInfo[i].co2);
        Filesupport::writeVectorList(pStream, m_mapVictoryInfo[i].score);
    }
}

void Userdata::deserializeObject(QDataStream& pStream)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
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
        for (qint32 i = 0; i < size; i++)
        {
            m_mapVictoryInfo.append(MapVictoryInfo());
            pStream >> m_mapVictoryInfo[i].mapPath;
            m_mapVictoryInfo[i].co1 = Filesupport::readVectorList<QString, QList>(pStream);
            m_mapVictoryInfo[i].co2 = Filesupport::readVectorList<QString, QList>(pStream);
            m_mapVictoryInfo[i].score = Filesupport::readVectorList<qint32, QVector>(pStream);
        }
    }
}
