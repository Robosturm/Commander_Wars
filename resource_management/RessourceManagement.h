#ifndef RESSOURCEMANAGEMENT_H
#define RESSOURCEMANAGEMENT_H

#include "QString"
#include "QStringList"
#include <QFileInfo>
#include <QDirIterator>
#include <QCoreApplication>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/interpreter.h"
#include "coreengine/settings.h"

template<class TClass>
class RessourceManagement : public oxygine::Resources
{
public:
    /**
     * @brief getInstance
     * @return
     */
    static TClass* getInstance();
    /**
     * @brief loadAll loads all units data
     */
    virtual void loadAll();
    /**
     * @brief getID
     * @param position
     * @return
     */
    QString getID(qint32 position);
    /**
     * @brief getIndex
     * @param id
     * @return
     */
    qint32 getIndex(QString id);
    /**
     * @brief getCount
     * @return
     */
    qint32 getCount()
    {
        return m_loadedRessources.size();
    }
    /**
     * @brief reset
     */
    virtual void reset();
    /**
     * @brief exists
     * @param id
     * @return
     */
    bool exists(QString id);
    /**
     * @brief getName
     * @param position
     * @return
     */
    QString getName(qint32 position);
    /**
     * @brief getName
     * @param id
     * @return
     */
    QString getName(QString id);
    /**
     * @brief getLoadedRessources
     * @return
     */
    QStringList getLoadedRessources()
    {
        return m_loadedRessources;
    }
protected:
    explicit RessourceManagement(QString resPath, QString scriptPath);
    virtual ~RessourceManagement() = default;
    QStringList m_loadedRessources;
    QString scriptPath;
    QStringList getSearchPaths();
    void loadRessources(QString resPath);
    void loadAll(QStringList& list);
    void reset(QStringList& list);
private:
    static TClass* m_pInstance;
};

template<class TClass>
TClass* RessourceManagement<TClass>::m_pInstance = nullptr;

template<class TClass>
TClass* RessourceManagement<TClass>::getInstance()
{
    if (m_pInstance == nullptr)
    {
        Interpreter::setCppOwnerShip(m_pInstance);
        m_pInstance = new TClass();
    }
    return m_pInstance;
}

template<class TClass>
RessourceManagement<TClass>::RessourceManagement(QString resPath, QString scriptPath)
    : scriptPath(scriptPath)
{
    loadRessources(resPath);
}

template<class TClass>
void RessourceManagement<TClass>::loadRessources(QString resPath)
{
    if (!resPath.isEmpty())
    {
        if (QFile::exists("resources/" + resPath))
        {
            oxygine::Resources::loadXML("resources/" + resPath);
        }
        for (qint32 i = 0; i < Settings::getMods().size(); i++)
        {
            if (QFile::exists(Settings::getMods().at(i) + resPath))
            {
                oxygine::Resources::loadXML(Settings::getMods().at(i) + resPath);
            }
        }
    }
}

template<class TClass>
QString RessourceManagement<TClass>::getID(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        return m_loadedRessources.at(position);
    }
    return "";
}

template<class TClass>
qint32 RessourceManagement<TClass>::getIndex(QString id)
{
    for (qint32 i = 0; i < m_loadedRessources.size(); i++)
    {
        if (m_loadedRessources[i] == id)
        {
            return i;
        }
    }
    return -1;
}

template<class TClass>
void RessourceManagement<TClass>::reset()
{
    reset(m_loadedRessources);
}

template<class TClass>
void RessourceManagement<TClass>::reset(QStringList& list)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < list.size(); i++)
    {
        pInterpreter->deleteObject(list[i]);
    }
    list.clear();
}

template<class TClass>
bool RessourceManagement<TClass>::exists(QString id)
{
    for (qint32 i = 0; i < m_loadedRessources.size(); i++)
    {
        if (m_loadedRessources[i] == id)
        {
            return true;
        }
    }
    return false;
}

template<class TClass>
QStringList RessourceManagement<TClass>::getSearchPaths()
{
    QStringList searchPaths;
    if (!scriptPath.isEmpty())
    {
        searchPaths.append("resources/" + scriptPath);
        // make sure to overwrite existing js stuff
        for (qint32 i = 0; i < Settings::getMods().size(); i++)
        {
            searchPaths.append(Settings::getMods().at(i) + "/" + scriptPath);
        }
    }
    return searchPaths;
}

template<class TClass>
void RessourceManagement<TClass>::loadAll()
{
    reset();
    loadAll(m_loadedRessources);
}

template<class TClass>
void RessourceManagement<TClass>::loadAll(QStringList& list)
{    
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths = getSearchPaths();
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString id = dirIter.fileInfo().fileName().split(".").at(0).toUpper();
            pInterpreter->openScript(dirIter.fileInfo().filePath(), true);
            if (!list.contains(id) &&
                !id.startsWith("__"))
            {
                list.append(id);
            }
        }
    }
    list.sort();
}

template<class TClass>
QString RessourceManagement<TClass>::getName(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        QString name = m_loadedRessources[position];
        QJSValueList args;
        args << name;
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(name, "getName", args);
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}

template<class TClass>
QString RessourceManagement<TClass>::getName(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction(id, "getName");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}
#endif // RESSOURCEMANAGEMENT_H
