#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qdir.h"
#include "qtextstream.h"

namespace oxygine
{
    Resources::registeredResources Resources::_registeredResources;
    ResAnim* _defaultMissingRS = nullptr;

    void Resources::registerResourceType(Resources::createResourceCallback creationCallback, QString resTypeID)
    {
        registeredResource r;
        r.cb = creationCallback;
        r.id = resTypeID;

        registeredResources::iterator it = std::lower_bound(_registeredResources.begin(), _registeredResources.end(), r.id, registeredResource::comparePred2);
        if (it != _registeredResources.end())
        {
            if (it->id == resTypeID)
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

    void Resources::unregisterResourceType(QString resTypeID)
    {
        registeredResources::iterator it = std::lower_bound(_registeredResources.begin(), _registeredResources.end(), resTypeID, registeredResource::comparePred2);
        if (it != _registeredResources.end())
        {
            if (it->id == resTypeID)
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

    ResAnim* Resources::getResAnim(QString id, error_policy ep) const
    {
        return getT<ResAnim>(id, ep, _defaultMissingRS);
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
        _docs.clear();
    }

    bool Resources::isEmpty() const
    {
        return _docs.empty();
    }

    void Resources::updateName(QString filename)
    {
        QFile file(filename);
        setName(file.fileName());
    }

    bool Resources::loadXML(const QString xmlFile, const ResourcesLoadOptions& opt)
    {
        _name = xmlFile;
        _loadCounter = opt._loadCompletely ? 1 : 0;


        qDebug("step0");
        QFile file(xmlFile);

        if (!file.exists() || file.size() == 0)
        {
            qCritical("can't load xml file: '%s'", xmlFile.toStdString().c_str());
            Q_ASSERT(!"can't find xml file");
            return false;
        }
        file.open(QIODevice::ReadOnly);
        qDebug("step1");
        updateName(xmlFile);
        qDebug("step2");


        _docs.push_back(QDomDocument());
        QDomDocument& doc = _docs.last();
        bool loaded = doc.setContent(&file);
        Q_ASSERT(loaded);

        QDomElement resources = doc.documentElement();
        QString id;
        qDebug("loading xml resources");
        XmlWalker walker("", 1.0f, opt._loadCompletely, true, resources);

        while (true)
        {
            CreateResourceContext context;
            context.options = &opt;
            context.walker = walker.next();
            if (context.walker.empty())
                break;

            QString type = context.walker.getType();

            registeredResources::iterator i = std::lower_bound(_registeredResources.begin(), _registeredResources.end(), type);
            if (i == _registeredResources.end() || (i->id != type))
            {
                qCritical("unknown resource. type: '%s' id: '%s'", type.toStdString().c_str(),
                          Resource::extractID(context.walker.getNode(), "", "").toStdString().c_str());
                Q_ASSERT(!"unknown resource type");
                continue;
            }

            registeredResource& r = *i;


            context.xml_name = xmlFile;
            context.resources = this;

            qDebug("resource: %s ", context.xml_name.toStdString().c_str());
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
            spResource res = i.value();
            r.push_back(res);
        }

    }

    void Resources::add(Resource* r, bool accessByShortenID)
    {
        Q_ASSERT(r);
        if (!r)
            return;

        QString name = r->getName().toLower();
        r->setName(name);
        _resourcesMap[name] = r;

        if (accessByShortenID)
        {
            QFileInfo file(name);
            QString shortName = file.fileName();
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
            spResource res = i.value();
            qDebug("%s", res->getName().toStdString().c_str());
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

    Resource* Resources::get(QString id_, error_policy ep, Resource* defIfNotFound) const
    {
        QString id = id_.toLower();

        resourcesMap::const_iterator it = _resourcesMap.find(id);

        if (it != _resourcesMap.end())
        {
            return it.value().get();
        }

        handleErrorPolicy(ep, "can't find resource: '" + id + "' in '" + _name + "'");
        if (ep == ep_show_error)
        {
            return defIfNotFound;
        }
        return nullptr;
    }

    void Resources::setLinearFilter(quint32 linearFilter)
    {
        for (auto & res : _resources)
        {
            res->setLinearFilter(linearFilter);
        }
    }

    quint32 Resources::getLinearFilter() const
    {
        if (_resources.size() > 0)
        {
            return _resources[0]->getLinearFilter();
        }
        return 0;
    }
}

