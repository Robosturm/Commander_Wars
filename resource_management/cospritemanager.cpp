#include <QFileInfo>

#include <QDirIterator>
#include "cospritemanager.h"
#include "coreengine/mainapp.h"

#include "spritingsupport/spritecreator.h"

COSpriteManager* COSpriteManager::m_pInstance = nullptr;

COSpriteManager::COSpriteManager()
{
    oxygine::Resources::loadXML("resources/images/co/res.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/co/res.xml"))
        {
            oxygine::Resources::loadXML(pMainapp->getSettings()->getMods().at(i) + "/images/co/res.xml");
        }
    }
}

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

COSpriteManager* COSpriteManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new COSpriteManager();
    }
    return m_pInstance;
}

QString COSpriteManager::getCOName(qint32 position)
{
    if ((position >= 0) && (position < m_loadedCOs.size()))
    {
        Mainapp* pApp = Mainapp::getInstance();
        Interpreter* pInterpreter = pApp->getInterpreter();
        QJSValue value = pInterpreter->doFunction(m_loadedCOs[position], "getName");
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}


QString COSpriteManager::getCOName(QString coid)
{
    Mainapp* pApp = Mainapp::getInstance();
    Interpreter* pInterpreter = pApp->getInterpreter();
    QJSValue value = pInterpreter->doFunction(coid, "getName");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}


QStringList COSpriteManager::getCOStyles(QString id)
{
    for (qint32 i = 0; i < m_loadedCOs.size(); i++)
    {
        if (m_loadedCOs[i] == id)
        {
            return getCOStyles(i);
        }
    }
    return QStringList();
}

QStringList COSpriteManager::getCOStyles(qint32 position)
{
    if ((position >= 0) && (position < m_loadedCOs.size()))
    {
        Mainapp* pApp = Mainapp::getInstance();
        Interpreter* pInterpreter = pApp->getInterpreter();
        QJSValue value = pInterpreter->doFunction(m_loadedCOs[position], "getCOStyles");
        return value.toVariant().toStringList();
    }
    return QStringList();
}

void COSpriteManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/cos");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/cos");
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter->hasNext())
        {
            dirIter->next();
            QString file = dirIter->fileInfo().fileName().split(".").at(0);
            if (!m_loadedCOs.contains(file.toUpper()))
            {
                loadCO(file.toUpper());
            }
        }
    }
    m_loadedCOs.sort();
}

qint32 COSpriteManager::getCOIndex(QString id)
{
    for (qint32 i = 0; i < m_loadedCOs.size(); i++)
    {
        if (m_loadedCOs[i] == id)
        {
            return i;
        }
    }
    return -1;
}

bool COSpriteManager::loadCO(QString coID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    searchPaths.append("resources/scripts/cos");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/cos");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + coID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            if (!bRet)
            {
                m_loadedCOs.append(coID);
            }
            bRet = true;
        }
    }
    return bRet;
}

void COSpriteManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedCOs.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedCOs[i]);
    }
    m_loadedCOs.clear();
}

bool COSpriteManager::existsCO(QString coID)
{
    for (qint32 i = 0; i < m_loadedCOs.size(); i++)
    {
        if (m_loadedCOs[i] == coID)
        {
            return true;
        }
    }
    return false;
}

void COSpriteManager::loadResAnim(QString coid, QString file, QImage& colorTable, QImage& maskTable, bool useColorBox)
{
    Mainapp* pApp = Mainapp::getInstance();
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
        m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+face",
                            SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor())));
    }
    if (!infoFound)
    {
        oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+info", oxygine::error_policy::ep_ignore_error);
        m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+info",
                            SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor())));
    }
    if (!nrmFound)
    {
        oxygine::ResAnim* pAnim = oxygine::Resources::getResAnim(coidLower + "+nrm", oxygine::error_policy::ep_ignore_error);
        m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+nrm",
                            SpriteCreator::createAnim(file + "+nrm.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor())));
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
