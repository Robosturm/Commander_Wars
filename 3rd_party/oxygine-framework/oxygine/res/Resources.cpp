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
    ResAnim* Resources::m_defaultMissingRS = nullptr;

    void Resources::registerResourceType(Resources::createResourceCallback creationCallback, QString resTypeID)
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

    void Resources::unregisterResourceType(QString resTypeID)
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

    void Resources::setDefaultMissingResAnim(ResAnim* rs)
    {
        m_defaultMissingRS = rs;
    }

    Resources::Resources()
    {
    }

    Resources::~Resources()
    {
        free();
    }

    ResAnim* Resources::getResAnim(QString id, error_policy ep) const
    {
        return getT<ResAnim>(id, ep, m_defaultMissingRS);
    }

    ResFont* Resources::getResFont(QString id, error_policy ep) const
    {
        return getT<ResFont>(id, ep);
    }

    void Resources::load()
    {
        Resource::load(nullptr);
    }

    void Resources::unload()
    {
        Resource::unload();
    }

    void Resources::_load(LoadResourcesContext* context)
    {
        for (resources::iterator i = m_resources.begin(); i != m_resources.end(); ++i)
        {
            Resource* res = (*i).get();
            res->load(context);
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

    void Resources::updateName(QString filename)
    {
        QFile file(filename);
        setName(file.fileName());
    }

    bool Resources::loadXML(const QString xmlFile, const ResourcesLoadOptions& opt)
    {
        m_name = xmlFile;
        m_loadCounter = opt.m_loadCompletely ? 1 : 0;
        QFile file(xmlFile);

        if (!file.exists() || file.size() == 0)
        {
            Console::print("can't load xml file: '" + xmlFile + "'", Console::eDEBUG);
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
            Console::print("loading xml resources", Console::eDEBUG);
            XmlWalker walker("", 1.0f, opt.m_loadCompletely, true, resources);
            while (true)
            {
                CreateResourceContext context;
                context.m_options = &opt;
                context.m_walker = walker.next();
                if (context.m_walker.empty())
                {
                    break;
                }
                QString type = context.m_walker.getType();

                registeredResources::iterator i = std::lower_bound(m_registeredResources.begin(), m_registeredResources.end(), type);
                if (i == m_registeredResources.end() || (i->id != type))
                {
                    Console::print("unknown resource. type: " + type + " id: " + Resource::extractID(context.m_walker.getNode(), "", ""), Console::eERROR);
                    oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::loadXML unable to determine ressource type");
                    continue;
                }

                registeredResource& r = *i;


                context.m_xml_name = xmlFile;
                context.m_resources = this;

                Console::print("resource: " + context.m_xml_name, Console::eDEBUG);
                spResource res = r.cb(context);
                if (res.get() == nullptr)
                {
                    oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::loadXML unable to load ressource");
                }
                res->setUseLoadCounter(opt.m_useLoadCounter);

                if (res)
                {
                    if (context.m_walker.getLoad())
                    {
                        res->load(nullptr);
                    }
                    res->setParent(this);
                    m_resources.push_back(res);
                }
            }
            Console::print("xml loaded", Console::eDEBUG);
        }
        else
        {
            Console::print("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
        }
        return true;
    }

    void Resources::collect(resources& r)
    {
        for (resourcesMap::const_iterator i = m_resourcesMap.cbegin(); i != m_resourcesMap.cend(); ++i)
        {
            spResource res = i.value();
            r.push_back(res);
        }

    }

    void Resources::add(spResource r, bool accessByShortenID)
    {
        if (r.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Resources::add adding empty ressource");
            return;
        }
        QString name = r->getName().toLower();
        r->setName(name);
        m_resourcesMap[name] = r;

        if (accessByShortenID)
        {
            QFileInfo file(name);
            QString shortName = file.fileName();
            if (shortName != name)
            {
                m_resourcesMap[shortName] = r;
            }
        }
    }

    Resources::resources& Resources::_getResources()
    {
        return m_resources;
    }

    Resources::resourcesMap& Resources::_getResourcesMap()
    {
        return m_resourcesMap;
    }

    Resource* Resources::get(QString id_, error_policy ep, Resource* defIfNotFound) const
    {
        QString id = id_.toLower();

        resourcesMap::const_iterator it = m_resourcesMap.find(id);

        if (it != m_resourcesMap.end())
        {
            return it.value().get();
        }

        handleErrorPolicy(ep, "can't find resource: '" + id + "' in '" + m_name + "'");
        if (ep == ep_show_error)
        {
            return defIfNotFound;
        }
        return nullptr;
    }

    void Resources::setLinearFilter(quint32 linearFilter)
    {
        for (auto & res : m_resources)
        {
            res->setLinearFilter(linearFilter);
        }
    }

    quint32 Resources::getLinearFilter() const
    {
        if (m_resources.size() > 0)
        {
            return m_resources[0]->getLinearFilter();
        }
        return 0;
    }
}

