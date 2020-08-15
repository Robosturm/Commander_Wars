#include <QFileInfo>

#include <QDirIterator>
#include "cospritemanager.h"
#include "coreengine/mainapp.h"

#include "spritingsupport/spritecreator.h"

QVector<QString> COSpriteManager::getSpriteCOIDs()
{
    QVector<QString> ret;
    for (auto iter = _resourcesMap.begin(); iter != _resourcesMap.cend(); ++iter)
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

void COSpriteManager::loadResAnim(QString coid, QString file, QImage& colorTable, QImage& maskTable, bool useColorBox)
{
    QString coidLower = coid.toLower();
    bool nrmFound = false;
    bool faceFound = false;
    bool infoFound = false;
    for (qint32 i = 0; i < m_Ressources.size(); i++)
    {
        if (coidLower+ "+nrm" == std::get<0>(m_Ressources[i]))
        {
            oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+nrm", oxygine::error_policy::ep_ignore_error);
            std::get<1>(m_Ressources[i]) = SpriteCreator::createAnim(file + "+nrm.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            nrmFound = true;
        }
        else if (coidLower + "+face" == std::get<0>(m_Ressources[i]))
        {
            oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+face", oxygine::error_policy::ep_ignore_error);
            std::get<1>(m_Ressources[i]) = SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            faceFound = true;
        }
        else if (coidLower + "+info" == std::get<0>(m_Ressources[i]))
        {
            oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+info", oxygine::error_policy::ep_ignore_error);
            std::get<1>(m_Ressources[i]) = SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            infoFound = true;
        }
    }

    if (!faceFound)
    {
        oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+face", oxygine::error_policy::ep_ignore_error);
        if (pAnim != nullptr)
        {
            oxygine::spResAnim pCOAnim = SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+face", pCOAnim));
            }
        }
    }
    if (!infoFound)
    {
        oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+info", oxygine::error_policy::ep_ignore_error);
        if (pAnim != nullptr)
        {
            oxygine::spResAnim pCOAnim = SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+info", pCOAnim));
            }
        }
    }
    if (!nrmFound)
    {
        oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+nrm", oxygine::error_policy::ep_ignore_error);
        if (pAnim != nullptr)
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
