#include <QFileInfo>
#include <QDirIterator>

#include "resource_management/cospritemanager.h"

#include "coreengine/mainapp.h"

#include "spritingsupport/spritecreator.h"

COSpriteManager::COSpriteManager()
    : RessourceManagement<COSpriteManager>("/images/co/res.xml",
                                           "/scripts/cos")
{
    setObjectName("COSpriteManager");
    connect(this, &COSpriteManager::sigLoadResAnim, this, &COSpriteManager::loadResAnim, Qt::QueuedConnection);
}

QVector<QString> COSpriteManager::getSpriteCOIDs()
{
    QVector<QString> ret;
    for (auto iter = m_resourcesMap.begin(); iter != m_resourcesMap.end(); ++iter)
    {
        QString id(iter.key());
        if (id.endsWith("+face"))
        {
            QString name = id.toUpper().replace("+FACE", "");
            if (!ret.contains(name))
            {
                ret.append(name);
            }
        }
    }
    return ret;
}

QStringList COSpriteManager::getCOStyles(QString id)
{
    for (qint32 i = 0; i < m_loadedRessources.size(); i++)
    {
        if (m_loadedRessources[i] == id)
        {
            return getCOStyles(i);
        }
    }
    return QStringList();
}

QStringList COSpriteManager::getCOStyles(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(m_loadedRessources[position], "getCOStyles");
        return value.toVariant().toStringList();
    }
    return QStringList();
}

void COSpriteManager::loadResAnim(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox)
{
    colorTable.convertTo(QImage::Format_ARGB32);
    maskTable.convertTo(QImage::Format_ARGB32);
    QString coidLower = coid.toLower();
    bool nrmFound = false;
    bool faceFound = false;
    bool infoFound = false;
    QStringList filenameList = file.split("/");
    QString filename = filenameList[filenameList.size() - 1];
    oxygine::spResAnim pCOAnim;
    oxygine::spResAnim pAnim;
    for (qint32 i = 0; i < m_Ressources.size(); i++)
    {
        pCOAnim = nullptr;
        pAnim = nullptr;
        if (coidLower+ "+nrm" == std::get<0>(m_Ressources[i]))
        {
            pAnim = oxygine::Resources::getResAnim(filename + "+nrm", oxygine::error_policy::ep_ignore_error);
            pCOAnim = SpriteCreator::createAnim(file + "+nrm.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            m_Ressources[i] = std::tuple<QString, oxygine::spResAnim>(coidLower + "+nrm", pCOAnim);
            nrmFound = true;
        }
        else if (coidLower + "+face" == std::get<0>(m_Ressources[i]))
        {
            pAnim = oxygine::Resources::getResAnim(filename + "+face", oxygine::error_policy::ep_ignore_error);
            pCOAnim = SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            m_Ressources[i] = std::tuple<QString, oxygine::spResAnim>(coidLower + "+face", pCOAnim);
            faceFound = true;
        }
        else if (coidLower + "+info" == std::get<0>(m_Ressources[i]))
        {
            pAnim = oxygine::Resources::getResAnim(filename + "+info", oxygine::error_policy::ep_ignore_error);
            pCOAnim = SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            m_Ressources[i] = std::tuple<QString, oxygine::spResAnim>(coidLower + "+info", pCOAnim);
            infoFound = true;
        }
    }
    pAnim = nullptr;
    pCOAnim = nullptr;
    if (!faceFound)
    {
        pAnim = oxygine::Resources::getResAnim(filename + "+face", oxygine::error_policy::ep_ignore_error);
        if (pAnim.get() != nullptr)
        {
            pCOAnim = SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+face", pCOAnim));
            }
        }
    }
    pAnim = nullptr;
    pCOAnim = nullptr;
    if (!infoFound)
    {
        pAnim = oxygine::Resources::getResAnim(filename + "+info", oxygine::error_policy::ep_ignore_error);
        if (pAnim.get() != nullptr)
        {
            pCOAnim = SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+info", pCOAnim));
            }
        }
    }
    pAnim = nullptr;
    pCOAnim = nullptr;
    if (!nrmFound)
    {
        oxygine::spResAnim pAnim = oxygine::Resources::getResAnim(filename + "+nrm", oxygine::error_policy::ep_ignore_error);
        if (pAnim.get() != nullptr)
        {
            oxygine::spResAnim pCOAnim = SpriteCreator::createAnim(file + "+nrm.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+nrm", pCOAnim));
            }
        }
    }
}

oxygine::ResAnim* COSpriteManager::getResAnim(QString id, oxygine::error_policy ep) const
{
    for (qint32 i = 0; i < m_Ressources.size(); i++)
    {
        if (id.toLower() == std::get<0>(m_Ressources[i]).toLower())
        {
            return std::get<1>(m_Ressources[i]).get();
        }
    }
    return oxygine::Resources::getResAnim(id, ep);
}
