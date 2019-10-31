#include "Resources.h"
#include "CreateResourceContext.h"
#include "ResAnim.h"
#include "ResFont.h"
#include "Resource.h"
#include "../pugixml/pugixml.hpp"
#include "../utils/stringUtils.h"
#include <algorithm>
#include <stdarg.h>
#include <stdio.h>

#include "qfile.h"
#include "qfileinfo.h"
#include "qdir.h"
#include "qtextstream.h"

namespace oxygine
{
    Resources::registeredResources Resources::_registeredResources;
    ResAnim* _defaultMissingRS = nullptr;

    void Resources::registerResourceType(Resources::createResourceCallback creationCallback, const char* resTypeID)
    {
        registeredResource r;
        r.cb = creationCallback;
        strcpy(r.id, resTypeID);

        registeredResources::iterator it = std::lower_bound(_registeredResources.begin(), _registeredResources.end(), r.id, registeredResource::comparePred2);
        if (it != _registeredResources.end())
        {
            if (!strcmp(it->id, resTypeID))
            {
                Q_ASSERT(!"resource already registered");
                return;
            }
        }
        _registeredResources.insert(it, r);

        /*
        for (it = _registeredResources.begin(); it != _registeredResources.end(); ++it)
        {
            registeredResource *q = &(*it);
            int er =0 ;
        }
        */
    }

    void Resources::unregisterResourceType(const char* resTypeID)
    {
        registeredResources::iterator it = std::lower_bound(_registeredResources.begin(), _registeredResources.end(), resTypeID, registeredResource::comparePred2);
        if (it != _registeredResources.end())
        {
            if (!strcmp(it->id, resTypeID))
            {
                _registeredResources.erase(it);
                return;
            }
        }
        Q_ASSERT(!"can't find resource type");
    }

    void Resources::setDefaultMissingResAnim(ResAnim* rs)
    {
        _defaultMissingRS = rs;
    }

    Resources::Resources()
    {
    }

    Resources::~Resources()
    {
        free();
    }

    ResAnim* Resources::getResAnim(const std::string& id, error_policy ep) const
    {
        return getT<ResAnim>(id, ep, _defaultMissingRS);
    }

    ResFont* Resources::getResFont(const std::string& id, error_policy ep) const
    {
        return getT<ResFont>(id, ep);
    }

    void Resources::load(ResLoadedCallback cb)
    {
        Resource::load(nullptr);
        //if (cb)
        //  cb(thi)
    }

    void Resources::unload()
    {
        Resource::unload();
    }

    void Resources::_load(LoadResourcesContext* context)
    {
        for (resources::iterator i = _resources.begin(); i != _resources.end(); ++i)
        {
            Resource* res = (*i).get();
            //qDebug("loading res: %s", res->getName().c_str());
            res->load(context);
            //if (cb)
            //  cb(res);
        }
    }

    void Resources::_unload()
    {
        for (resources::iterator i = _resources.begin(); i != _resources.end(); ++i)
        {
            Resource* res = (*i).get();
            res->unload();
        }
    }

    void Resources::free()
    {
        _resourcesMap.clear();
        _resources.clear();

        for (size_t i = 0; i < _docs.size(); ++i)
            delete _docs[i];
        _docs.clear();
    }

    bool Resources::isEmpty() const
    {
        return _docs.empty();
    }

    void Resources::updateName(const std::string& filename)
    {
        QFile file(filename.c_str());
        setName(file.fileName().toStdString());
    }


    class ObjectBasePredicate
    {
    public:
        bool operator()(const ObjectBase* res, const char* name) const
        {
            return strcmp(res->getName().c_str(), name) < 0;
        }

        bool operator()(const spResource& res, const char* name) const
        {
            return strcmp(res->getName().c_str(), name) < 0;
        }

        bool operator()(const spResource& resA, const spResource& resB) const
        {
            return strcmp(resA->getName().c_str(), resB->getName().c_str()) < 0;
        }

        bool operator()(const char* name, const ObjectBase* res) const
        {
            return strcmp(res->getName().c_str(), name) < 0;
        }

        bool operator()(const ObjectBase* resA, const ObjectBase* resB) const
        {
            return strcmp(resA->getName().c_str(), resB->getName().c_str()) < 0;
        }
    };


    bool Resources::loadXML(const std::string& xmlFile, const ResourcesLoadOptions& opt)
    {
        _name = xmlFile;
        _loadCounter = opt._loadCompletely ? 1 : 0;


        qDebug("step0");
        QFile file(xmlFile.c_str());

        if (!file.exists() || file.size() == 0)
        {
            qCritical("can't load xml file: '%s'", xmlFile.c_str());
            Q_ASSERT(!"can't find xml file");
            return false;
        }
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        std::string data = stream.readAll().toStdString();
        std::vector<uchar> fb(data.begin(), data.end());

        qDebug("step1");


        updateName(xmlFile);
        QDir dir = QFileInfo(xmlFile.c_str()).dir();
        qDebug("step2");

        pugi::xml_document* doc = new pugi::xml_document();
        _docs.push_back(doc);

        bool loaded = doc->load_buffer(&fb[0], fb.size());
        Q_ASSERT(loaded);

        pugi::xml_node resources = doc->first_child();
        pugi::xml_node resources_meta = pugi::xml_node();

        std::string id;

        qDebug("loading xml resources");

        std::string xmlFolder = dir.path().toStdString();
        XmlWalker walker(&xmlFolder, "", 1.0f, opt._loadCompletely, true, resources, resources_meta);

        while (true)
        {
            CreateResourceContext context;
            context.options = &opt;
            context.walker = walker.next();
            if (context.walker.empty())
                break;


            const char* type = context.walker.getType();

            registeredResources::iterator i = std::lower_bound(_registeredResources.begin(), _registeredResources.end(), type);
            if (i == _registeredResources.end() || strcmp(i->id, type))
            {
                qCritical("unknown resource. type: '%s' id: '%s'", type, Resource::extractID(context.walker.getNode(), "", "").c_str());
                Q_ASSERT(!"unknown resource type");
                continue;
            }

            registeredResource& r = *i;


            context.xml_name = &xmlFile;
            context.resources = this;

            qDebug("resource: %s ", context.xml_name->c_str());
            Resource* res = r.cb(context);
            Q_ASSERT(res);
            res->setUseLoadCounter(opt._useLoadCounter);

            if (res)
            {
                if (context.walker.getLoad())
                {
                    res->load(nullptr);
                }
                res->setParent(this);
                _resources.push_back(res);
            }
        }

        qDebug("xml loaded");
        return true;
    }

    void Resources::collect(resources& r)
    {
        for (resourcesMap::const_iterator i = _resourcesMap.cbegin(); i != _resourcesMap.cend(); ++i)
        {
            spResource res = i->second;
            r.push_back(res);
        }

    }

    void Resources::add(Resource* r, bool accessByShortenID)
    {
        Q_ASSERT(r);
        if (!r)
            return;

        std::string name = lower(r->getName());
        r->setName(name);
        _resourcesMap[name] = r;

        if (accessByShortenID)
        {
            QFileInfo file(name.c_str());
            std::string shortName = file.fileName().toStdString();
            if (shortName != name)
            {
                _resourcesMap[shortName] = r;
            }
        }
    }


    void Resources::print() const
    {
        qDebug("resources:\n");
        for (resourcesMap::const_iterator i = _resourcesMap.cbegin(); i != _resourcesMap.cend(); ++i)
        {
            spResource res = i->second;
            qDebug("%s", res->getName().c_str());
        }

        /*
        unsigned n = _resourcesMap.bucket_count();

        for (unsigned i=0; i<n; ++i) {
            qDebug("bucket %d: ", i);
            for (auto it = _resourcesMap.begin(i); it!=_resourcesMap.end(i); ++it)
                qDebug("%s, ", it->first.c_str());
            qDebug(" ");
        }
        */
    }

    Resources::resources& Resources::_getResources()
    {
        return _resources;
    }

    Resources::resourcesMap& Resources::_getResourcesMap()
    {
        return _resourcesMap;
    }

    Resource* Resources::get(const std::string& id_, error_policy ep, Resource* defIfNotFound) const
    {
        std::string id = lower(id_);

        resourcesMap::const_iterator it = _resourcesMap.find(id);

        if (it != _resourcesMap.end())
        {
            return it->second.get();
        }

        handleErrorPolicy(ep, "can't find resource: '%s' in '%s'", id.c_str(), _name.c_str());
        if (ep == ep_show_error)
            return defIfNotFound;
        return 0;
    }
}
