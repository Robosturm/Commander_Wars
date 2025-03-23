#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

#include "coreengine/gameconsole.h"

#include <QFile>

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
        return getT<ResAnim>(id, ep).get();
    }

    spResAnim Resources::getSpResAnim(const QString & id, error_policy ep) const
    {
        return getT<ResAnim>(id, ep);
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

    bool Resources::loadXML(const QString & xmlFile)
    {
#ifdef GRAPHICSUPPORT
        m_name = xmlFile;
        QFile file(xmlFile);

        if (!file.exists() || file.size() == 0)
        {
            CONSOLE_PRINT_MODULE("can't load xml file: '" + xmlFile + "'", GameConsole::eDEBUG, GameConsole::eResources);
            oxygine::handleErrorPolicy(oxygine::ep_ignore_error, "Resources::loadXML can't find xml file: " + xmlFile);
            return false;
        }
        file.open(QIODevice::ReadOnly);
        updateName(xmlFile);
        m_docs.push_back(QDomDocument());
        QDomDocument& doc = m_docs.last();
        auto result = doc.setContent(&file);
        if (result)
        {
            QDomElement resources = doc.documentElement();
            CONSOLE_PRINT_MODULE("loading xml resources", GameConsole::eDEBUG, GameConsole::eResources);
            XmlWalker walker("", 1.0f, resources);
            while (true)
            {
                CreateResourceContext context;
                context.m_walker = walker.next();
                if (context.m_walker.empty())
                {
                    break;
                }
                QString type = context.m_walker.getType();

                registeredResources::iterator i = std::lower_bound(m_registeredResources.begin(), m_registeredResources.end(), type);
                if (i == m_registeredResources.end() || (i->id != type))
                {
                    CONSOLE_PRINT("unknown resource. type: " + type + " id: " + Resource::extractID(context.m_walker.getNode(), "", ""), GameConsole::eERROR);
                    oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::loadXML unable to determine ressource type");
                    continue;
                }

                registeredResource& r = *i;


                context.m_xml_name = xmlFile;
                context.m_resources = this;

                CONSOLE_PRINT_MODULE("resource: " + context.m_xml_name, GameConsole::eDEBUG, GameConsole::eResources);
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
            CONSOLE_PRINT_MODULE("xml loaded", GameConsole::eDEBUG, GameConsole::eResources);
        }
        else
        {
            CONSOLE_PRINT("Error: " + result.errorMessage + " at line " + QString::number(result.errorLine) + " at column " + QString::number(result.errorColumn), GameConsole::eERROR);
        }
#endif
        return true;
    }

    void Resources::add(spResource r)
    {
        if (r.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::add adding empty ressource", GameConsole::eResources);
            return;
        }
        QString name = r->getName().toLower();
        if (name.isEmpty())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "adding resource with no name", GameConsole::eResources);
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

    spResource Resources::get(const QString & id_, error_policy ep) const
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
            return it.value();
        }
        handleErrorPolicy(ep, "can't find resource: '" + id + "' in '" + m_name + "'", GameConsole::eResources);
#endif
        return spResource();
    }

}

