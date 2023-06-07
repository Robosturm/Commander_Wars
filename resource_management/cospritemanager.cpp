#include <QFileInfo>
#include <QDirIterator>

#include "resource_management/cospritemanager.h"

#include "spritingsupport/spritecreator.h"

#include "game/co.h"

COSpriteManager::COSpriteManager()
    : RessourceManagement<COSpriteManager>("/images/co/res.xml",
                                           "/scripts/cos")
{
#ifdef GRAPHICSUPPORT
    setObjectName("COSpriteManager");
#endif
    Interpreter::setCppOwnerShip(this);
    connect(this, &COSpriteManager::sigLoadResAnim, this, &COSpriteManager::loadResAnim, Qt::QueuedConnection);
}

void COSpriteManager::release()
{
    m_Ressources.clear();
}

QStringList COSpriteManager::getSpriteCOIDs()
{
    QStringList ret;
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

QStringList COSpriteManager::getCOStyles(const QString id)
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

void COSpriteManager::loadResAnim(const QString coid, const QString file, QImage colorTable, QImage maskTable, bool useColorBox)
{
    colorTable.convertTo(QImage::Format_ARGB32);
    maskTable.convertTo(QImage::Format_ARGB32);
    QString coidLower = coid.toLower();
    bool nrmFound = false;
    bool faceFound = false;
    bool infoFound = false;
    bool miniFound = false;
    QStringList filenameList = file.split("/");
    QString filename = filenameList[filenameList.size() - 1];
    oxygine::spResAnim pCOAnim;
    oxygine::spResAnim pAnim;
    for (qint32 i = 0; i < m_Ressources.size(); i++)
    {
        pCOAnim.free();
        pAnim.free();
        if (coidLower+ "+nrm" == m_Ressources[i].m_spriteId)
        {
            pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+nrm", oxygine::error_policy::ep_ignore_error));
            pCOAnim = SpriteCreator::createAnim(file + "+nrm.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            m_Ressources[i] = CoSprite(coidLower + "+nrm", pCOAnim);
            nrmFound = true;
        }
        else if (coidLower + "+face" == m_Ressources[i].m_spriteId)
        {
            pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+face", oxygine::error_policy::ep_ignore_error));
            pCOAnim = SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            m_Ressources[i] = CoSprite(coidLower + "+face", pCOAnim);
            faceFound = true;
        }
        else if (coidLower + "+info" == m_Ressources[i].m_spriteId)
        {
            pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+info", oxygine::error_policy::ep_ignore_error));
            pCOAnim = SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            m_Ressources[i] = CoSprite(coidLower + "+info", pCOAnim);
            infoFound = true;
        }
        else if (coidLower + "+mini" == m_Ressources[i].m_spriteId)
        {
            pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+mini", oxygine::error_policy::ep_ignore_error));
            if (pAnim.get() != nullptr)
            {
                pCOAnim = SpriteCreator::createAnim(file + "+mini.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
                m_Ressources[i] = CoSprite(coidLower + "+mini", pCOAnim);
                infoFound = true;
            }
        }
    }
    pAnim.free();
    pCOAnim.free();
    if (!faceFound)
    {
        pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+face", oxygine::error_policy::ep_ignore_error));
        if (pAnim.get() != nullptr)
        {
            pCOAnim = SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(CoSprite(coidLower + "+face", pCOAnim));
            }
        }
    }
    pAnim.free();
    pCOAnim.free();
    if (!infoFound)
    {
        pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+info", oxygine::error_policy::ep_ignore_error));
        if (pAnim.get() != nullptr)
        {
            pCOAnim = SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(CoSprite(coidLower + "+info", pCOAnim));
            }
        }
    }
    pAnim.free();
    pCOAnim.free();
    if (!nrmFound)
    {
        oxygine::spResAnim pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+nrm", oxygine::error_policy::ep_ignore_error));
        if (pAnim.get() != nullptr)
        {
            oxygine::spResAnim pCOAnim = SpriteCreator::createAnim(file + "+nrm.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(CoSprite(coidLower + "+nrm", pCOAnim));
            }
        }
    }
    pAnim.free();
    pCOAnim.free();
    if (!miniFound)
    {
        oxygine::spResAnim pAnim = oxygine::spResAnim(oxygine::Resources::getResAnim(filename + "+mini", oxygine::error_policy::ep_ignore_error));
        if (pAnim.get() != nullptr)
        {
            oxygine::spResAnim pCOAnim = SpriteCreator::createAnim(file + "+mini.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
            if (pCOAnim.get() != nullptr)
            {
                m_Ressources.append(CoSprite(coidLower + "+mini", pCOAnim));
            }
        }
    }
}

oxygine::ResAnim* COSpriteManager::getResAnim(const QString & id, oxygine::error_policy ep) const
{
    for (qint32 i = 0; i < m_Ressources.size(); i++)
    {
        if (id.toLower() == m_Ressources[i].m_spriteId.toLower())
        {
            return m_Ressources[i].m_sprite.get();
        }
    }
    return oxygine::Resources::getResAnim(id, ep);
}

void COSpriteManager::removeRessource(const QString id)
{
    for (qint32 i = 0; i < m_loadedRessources.size(); ++i)
    {
        if (m_loadedRessources[i] == id)
        {
            m_loadedRessources.removeAt(i);
            break;
        }
    }
}

QStringList COSpriteManager::getArmyList(const QStringList coids) const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getArmies";
    QJSValue ret = pInterpreter->doFunction("PLAYER", function1);
    QStringList armies = ret.toVariant().toStringList();
    QStringList allowedArmies;
    // remove unused armies
    // first search avaible armies if the selection is set like that
    for (qint32 i = 0; i < coids.size(); i++)
    {
        QString function1 = "getCOArmy";
        QJSValue ret = pInterpreter->doFunction(coids[i], function1);
        if (ret.isString())
        {
            QString army = ret.toString();
            if (!allowedArmies.contains(army))
            {
                allowedArmies.append(army);
            }
        }
    }
    // we have allowed armies? Else allow everything
    if (allowedArmies.size() > 0)
    {
        // remove all other armies
        qint32 iter = 0;
        while (iter < armies.size())
        {
            if (allowedArmies.contains(armies[iter]))
            {
                iter++;
            }
            else
            {
                armies.removeAt(iter);
            }
        }
    }
    return armies;
}

QVector<COSpriteManager::CoGroup> COSpriteManager::getCoGroups(QStringList & coids)
{
    QVector<CoGroup> retCoGroups;
    // load default army and co sets
    Interpreter* pInterpreter = Interpreter::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    QJSValue ret = pInterpreter->doFunction("PLAYER", "getArmies");
    QStringList armies = ret.toVariant().toStringList();
    ret = pInterpreter->doFunction("PLAYER", "getArmyNames");
    QStringList armyNames = ret.toVariant().toStringList();
    coids.clear();
    // go through armies
    for (qint32 i = 0; i < armies.size(); ++i)
    {
        CoGroup newGroup;
        if (i < armyNames.size())
        {
            newGroup.name = armyNames[i];
        }
        else
        {
            newGroup.name = tr("Unknown");
        }
        // add default co order
        ret = pInterpreter->doFunction("PLAYER", "getArmyCOs" + armies[i]);
        auto coList = ret.toVariant().toStringList();
        coList.removeAll(CO::CO_RANDOM);
        coids.append(coList);
        newGroup.cos.append(coList);

        // add unadded co's of this army
        for (qint32 i2 = 0; i2 < pCOSpriteManager->getCount(); i2++)
        {
            QString coID = pCOSpriteManager->getID(i2);
            if (coID != CO::CO_RANDOM)
            {
                QString function1 = "getCOArmy";
                QJSValue ret = pInterpreter->doFunction(coID, function1);
                if (ret.isString())
                {
                    QString COArmy = ret.toString();
                    if (COArmy == armies[i] && !coids.contains(coID))
                    {
                        coids.append(coID);
                        newGroup.cos.append(coID);
                    }
                }
            }
        }
        retCoGroups.append(newGroup);
    }
    CoGroup unknownGroup;
    unknownGroup.name = tr("Unknown");
    // add unadded co's
    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        QString coID = pCOSpriteManager->getID(i);
        if (!coids.contains(coID) &&
            coID != CO::CO_RANDOM)
        {
            coids.append(coID);
            unknownGroup.cos.append(coids);
        }
    }
    if (unknownGroup.cos.length() > 0)
    {
        retCoGroups.append(unknownGroup);
    }
    return retCoGroups;
}
