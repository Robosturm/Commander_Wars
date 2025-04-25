#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QSet>

#include "coreengine/virtualpaths.h"

QList<VirtualPaths::SearchPathInfo> VirtualPaths::m_searchPath;

void VirtualPaths::setSearchPath(const QString& userPath, const QStringList& mods)
{
    CONSOLE_PRINT("Initializing VFS...", GameConsole::eINFO);

    m_searchPath.clear();

    m_searchPath.append({ userPath });

#ifndef USEAPPCONFIGPATH
    // USEAPPCONFIGPATH is primarily set on Linux, where the "current directory" of programs launched from the start
    // menu is normally the user's home directory. This is very unexpected behavior, and this should not be checked.
    if (QFileInfo(".") != QFileInfo(userPath))
    {
        m_searchPath.append({ "." });
    }
#endif

    for (const auto & mod : std::as_const(mods))
    {
        m_searchPath.append({ userPath + mod, true });
        m_searchPath.append({ oxygine::Resource::RCC_PREFIX_PATH + mod, true });
    }

#ifdef DEPLOY_RESOURCES_AS_FOLDER
    m_searchPath.append({ QCoreApplication::applicationDirPath() });

#ifdef __linux__
    m_searchPath.append({ QCoreApplication::applicationDirPath() + "/../share/commander_wars" });
#endif
#endif

    m_searchPath.append({ oxygine::Resource::RCC_PREFIX_PATH });

    for (auto & path : m_searchPath)
    {
        CONSOLE_PRINT("- Path: " + path.root + (path.isModPath ? " (mod data)" : ""), GameConsole::eINFO);
    }
}

VirtualPaths::ProcessedName VirtualPaths::processName(const QString& pName)
{
    // Strip leading slashes
    QString name = pName;
    while (name.startsWith("/"))
    {
        name = name.last(name.length() - 1);
    }
    // Search for the root directory itself.
    ProcessedName result{""};
    // Search for the resources directory itself.
    if (name == "resources" || name == "resources/")
    {
        result = ProcessedName { "/" + name, name.last(name.length() - 9), true };
    }
    // Search for a file in the resources directory
    else if (name.startsWith("resources/"))
    {
        result = ProcessedName { "/" + name, name.last(name.length() - 9), true };
    }
    // Search for a file anywhere else
    else
    {
        result = ProcessedName { "/" + name };
    }
    return result;
}

QString VirtualPaths::find(const QString& pName, bool checkMods)
{
    auto name = processName(pName);

    QString newPath;
    for (auto & path : m_searchPath)
    {
        if (!checkMods && path.isModPath)
        {
            continue;
        }

        newPath = path.root + name.path;
        if (QFileInfo(newPath).exists())
        {
            return newPath;
        }
        else if (name.isResources && path.isModPath)
        {
            newPath = path.root + name.resourcesPath;
            if (QFileInfo(newPath).exists())
            {
                return newPath;
            }
        }
    }
    return "";
}

QStringList VirtualPaths::createSearchPathInternal(const QString& pName, bool checkMods, bool firstPriority)
{
    auto name = processName(pName);

    QStringList list;
    QString newPath;
    for (auto & path : m_searchPath)
    {
        if (!checkMods && path.isModPath)
        {
            continue;
        }

        newPath = path.root + name.path;
        list.append(newPath);

        if (name.isResources && path.isModPath)
        {
            newPath = path.root + name.resourcesPath;
            list.append(newPath);
        }
    }

    if (!firstPriority)
    {
        std::reverse(list.begin(), list.end());
    }
    return list;
}

QStringList VirtualPaths::findAllInternal(const QString& pName, bool checkMods, bool firstPriority)
{
    auto name = processName(pName);

    QStringList list;
    QString newPath;
    for (auto & path : m_searchPath)
    {
        if (!checkMods && path.isModPath)
        {
            continue;
        }

        newPath = path.root + name.path;
        if (QFileInfo(newPath).exists())
        {
            list.append(newPath);
        }

        if (name.isResources && path.isModPath)
        {
            newPath = path.root + name.resourcesPath;
            if (QFileInfo(newPath).exists())
            {
                list.append(newPath);
            }
        }
    }

    if (!firstPriority)
    {
        std::reverse(list.begin(), list.end());
    }
    return list;
}

QFileInfoList VirtualPaths::list(const QString& name, const QStringList& filters, bool checkMods)
{
    QSet<QString> foundBaseNames;
    QFileInfoList infoList;
    QDir upDir(name + "/..");
    if (upDir.exists())
    {
        infoList.append(QFileInfo(upDir.dirName()));
    }
    for (auto & path : VirtualPaths::createSearchPathRev(name, checkMods))
    {
        QFileInfo pathInfo(path);
        if (pathInfo.exists() && pathInfo.isDir())
        {
            for (const auto & entry : QDir(path).entryInfoList(QDir::Dirs))
            {
                if (!foundBaseNames.contains(entry.baseName()))
                {
                    infoList.append(entry);
                    foundBaseNames.insert(entry.baseName());
                }
            }

            if (!filters.isEmpty())
            {
                for (const auto & entry : QDir(path).entryInfoList(filters, QDir::Files))
                {
                    if (!foundBaseNames.contains(entry.baseName()))
                    {
                        infoList.append(entry);
                        foundBaseNames.insert(entry.baseName());
                    }
                }
            }
        }
    }

    return infoList;
}
