#include "userdata.h"

#include "resource_management/cospritemanager.h"

#include "qfile.h"

#include "coreengine/settings.h"

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
    changeUser();
}

void Userdata::storeUser()
{
    QFile user(Settings::getUsername() + ".dat");
    user.remove();
    user.open(QIODevice::WriteOnly);
    QDataStream pStream(&user);
    serializeObject(pStream);
    user.close();
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
            return;
        }
    }
    m_customCOStyles.append(std::tuple<QString, QString, QImage, QImage, bool>(coid, file, colorTable, maskTable, useColorBox));
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

void Userdata::serializeObject(QDataStream& pStream)
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
}
