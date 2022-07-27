#ifndef RESSOURCEMANAGEMENT_H
#define RESSOURCEMANAGEMENT_H

#include "QString"
#include "QStringList"
#include <QFileInfo>
#include <QDirIterator>
#include <QCoreApplication>

#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"

#include "coreengine/interpreter.h"
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"


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
    qint32 getIndex(const QString & id);
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
    QString getName(const QString & id);
    /**
     * @brief getLoadedRessources
     * @return
     */
    QStringList getLoadedRessources()
    {
        return m_loadedRessources;
    }
    bool getLoaded() const;

protected:
    explicit RessourceManagement(QString resPath, QString scriptPath, bool addTransparentBorder = true);
    virtual ~RessourceManagement() = default;
    void loadRessources(QString resPath, bool addTransparentBorder = true);
    void loadAll(QStringList& list);
    void reset(QStringList& list);
    QStringList getSearchPaths();
protected:
    QStringList m_loadedRessources;
    QString m_scriptPath;
    bool m_loaded{false};
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
        m_pInstance = new TClass();
        Interpreter::setCppOwnerShip(m_pInstance);
    }
    return m_pInstance;
}

template<class TClass>
RessourceManagement<TClass>::RessourceManagement(QString resPath, QString scriptPath, bool addTransparentBorder)
    : m_scriptPath(scriptPath)
{
    loadRessources(resPath, addTransparentBorder);
}

template<class TClass>
void RessourceManagement<TClass>::loadRessources(QString resPath, bool addTransparentBorder)
{
    if (!resPath.isEmpty() && !Mainapp::getInstance()->getNoUi())
    {
        if (QFile::exists(QString(RCC_PREFIX_PATH) + "resources/" + resPath))
        {
            oxygine::Resources::loadXML(QString(RCC_PREFIX_PATH) + "resources/" + resPath, addTransparentBorder);
        }
        if (QFile::exists(Settings::getUserPath() + "resources/" + resPath))
        {
            oxygine::Resources::loadXML(Settings::getUserPath() + "resources/" + resPath, addTransparentBorder);
        }
        for (qint32 i = 0; i < Settings::getMods().size(); i++)
        {
            if (QFile::exists(QString(RCC_PREFIX_PATH) + Settings::getMods().at(i) + resPath))
            {
                oxygine::Resources::loadXML(QString(RCC_PREFIX_PATH) + Settings::getMods().at(i) + resPath, addTransparentBorder);
            }
            if (QFile::exists(Settings::getUserPath() + Settings::getMods().at(i) + resPath))
            {
                oxygine::Resources::loadXML(Settings::getUserPath() + Settings::getMods().at(i) + resPath, addTransparentBorder);
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
qint32 RessourceManagement<TClass>::getIndex(const QString & id)
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
    if (!m_scriptPath.isEmpty())
    {
        searchPaths.append(QString(RCC_PREFIX_PATH) + "resources/" + m_scriptPath);
        searchPaths.append(Settings::getUserPath() + "resources/" + m_scriptPath);
        // make sure to overwrite existing js stuff
        for (qint32 i = 0; i < Settings::getMods().size(); i++)
        {
            searchPaths.append(QString(RCC_PREFIX_PATH) + Settings::getMods().at(i) + "/" + m_scriptPath);
            searchPaths.append(Settings::getUserPath() + Settings::getMods().at(i) + "/" + m_scriptPath);
        }
    }
    return searchPaths;
}

template<class TClass>
bool RessourceManagement<TClass>::getLoaded() const
{
    return m_loaded;
}

template<class TClass>
void RessourceManagement<TClass>::loadAll()
{
    reset();
    loadAll(m_loadedRessources);
    m_loaded = true;
}

template<class TClass>
void RessourceManagement<TClass>::loadAll(QStringList& list)
{    
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths = getSearchPaths();
    QApplication::processEvents();
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path = searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString id = dirIter.fileInfo().fileName().split(".").at(0).toUpper();
            if (id.startsWith("__") &&
               !id.startsWith("___"))
            {
                pInterpreter->openScript(dirIter.fileInfo().filePath(), true);
            }
        }
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path = searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString id = dirIter.fileInfo().fileName().split(".").at(0).toUpper();
            if (!id.startsWith("__") &&
                !id.startsWith("___"))
            {
                pInterpreter->openScript(dirIter.fileInfo().filePath(), true);
                if (!list.contains(id))
                {
                    list.append(id);
                }
            }
        }
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path = searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString id = dirIter.fileInfo().fileName().split(".").at(0).toUpper();
            if (id.startsWith("___"))
            {
                pInterpreter->openScript(dirIter.fileInfo().filePath(), true);
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
QString RessourceManagement<TClass>::getName(const QString & id)
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
