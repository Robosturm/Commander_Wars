#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

#include "qfile.h"
#include "qfileinfo.h"
#include "qdir.h"
#include "qtextstream.h"

#include "coreengine/console.h"

namespace oxygine
{
    Resources::registeredResources Resources::m_registeredResources;

    void Resources::registerResourceType(Resources::createResourceCallback creationCallback, const QString & resTypeID)
    {
        registeredResource r;
        r.cb = creationCallback;
        r.id = resTypeID;

        auto it = std::lower_bound(m_registeredResources.cbegin(), m_registeredResources.cend(), r.id, registeredResource::comparePred2);
        if (it != m_registeredResources.cend())
        {
            if (it->id == resTypeID)
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "resource already registered");
                return;
            }
        }
        m_registeredResources.insert(it, r);
    }

    void Resources::unregisterResourceType(const QString & resTypeID)
    {
        auto it = std::lower_bound(m_registeredResources.cbegin(), m_registeredResources.cend(), resTypeID, registeredResource::comparePred2);
        if (it != m_registeredResources.cend())
        {
            if (it->id == resTypeID)
            {
                m_registeredResources.erase(it);
                return;
            }
        }
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "can't find resource type");
    }

    Resources::~Resources()
    {
        free();
    }

    ResAnim* Resources::getResAnim(const QString & id, error_policy ep) const
    {
        return getT<ResAnim>(id, ep);
    }

    ResFont* Resources::getResFont(const QString & id, error_policy ep) const
    {
        return getT<ResFont>(id, ep);
    }

    void Resources::load()
    {
        Resource::load();
    }

    void Resources::unload()
    {
        Resource::unload();
    }

    void Resources::_load()
    {
        for (resources::iterator i = m_resources.begin(); i != m_resources.end(); ++i)
        {
            Resource* res = (*i).get();
            res->load();
        }
    }

    void Resources::_unload()
    {
        for (resources::iterator i = m_resources.begin(); i != m_resources.end(); ++i)
        {
            Resource* res = (*i).get();
            res->unload();
        }
    }

    void Resources::free()
    {
        m_resourcesMap.clear();
        m_resources.clear();
        m_docs.clear();
    }

    bool Resources::isEmpty() const
    {
        return m_docs.empty();
    }

    void Resources::updateName(const QString & filename)
    {
        QFile file(filename);
        setName(file.fileName());
    }

    bool Resources::loadXML(const QString & xmlFile, bool addTransparentBorder)
    {
#ifdef GRAPHICSUPPORT
        m_name = xmlFile;
        QFile file(xmlFile);

        if (!file.exists() || file.size() == 0)
        {
            CONSOLE_PRINT("can't load xml file: '" + xmlFile + "'", Console::eDEBUG);
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::loadXML can't find xml file");
            return false;
        }
        file.open(QIODevice::ReadOnly);
        updateName(xmlFile);
        m_docs.push_back(QDomDocument());
        QDomDocument& doc = m_docs.last();
        QString error;
        qint32 line;
        qint32 column;
        bool loaded = doc.setContent(&file, &error, &line, &column);
        if (loaded)
        {
            QDomElement resources = doc.documentElement();
            CONSOLE_PRINT("loading xml resources", Console::eDEBUG);
            XmlWalker walker("", 1.0f, resources);
            while (true)
            {
                CreateResourceContext context;
                context.m_addTransparentBorder = addTransparentBorder;
                context.m_walker = walker.next();
                if (context.m_walker.empty())
                {
                    break;
                }
                QString type = context.m_walker.getType();

                registeredResources::iterator i = std::lower_bound(m_registeredResources.begin(), m_registeredResources.end(), type);
                if (i == m_registeredResources.end() || (i->id != type))
                {
                    CONSOLE_PRINT("unknown resource. type: " + type + " id: " + Resource::extractID(context.m_walker.getNode(), "", ""), Console::eERROR);
                    oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::loadXML unable to determine ressource type");
                    continue;
                }

                registeredResource& r = *i;


                context.m_xml_name = xmlFile;
                context.m_resources = this;

                CONSOLE_PRINT("resource: " + context.m_xml_name, Console::eDEBUG);
                spResource res = r.cb(context);
                if (res.get() == nullptr)
                {
                    oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::loadXML unable to load ressource");
                }
                if (res)
                {
                    res->load();
                    res->setParent(this);
                    m_resources.push_back(res);
                }
            }
            CONSOLE_PRINT("xml loaded", Console::eDEBUG);
        }
        else
        {
            CONSOLE_PRINT("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
        }
#endif
        return true;
    }

    void Resources::add(spResource r)
    {
        if (r.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::add adding empty ressource");
            return;
        }
        QString name = r->getName().toLower();
        if (name.isEmpty())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "adding resource with no name");
        }
        r->setName(name);
        m_resourcesMap[name] = r;
    }

    Resources::resources& Resources::_getResources()
    {
        return m_resources;
    }

    Resources::resourcesMap& Resources::_getResourcesMap()
    {
        return m_resourcesMap;
    }

    Resource* Resources::get(const QString & id_, error_policy ep) const
    {
#ifdef GRAPHICSUPPORT
        if (id_.isEmpty())
        {
            return nullptr;
        }
        QString id = id_.toLower();
        resourcesMap::const_iterator it = m_resourcesMap.find(id);
        if (it != m_resourcesMap.end())
        {
            return it.value().get();
        }
        handleErrorPolicy(ep, "can't find resource: '" + id + "' in '" + m_name + "'");
#endif
        return nullptr;
    }

}

