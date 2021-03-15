#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/closure/closure.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include <QDomDocument>
#include <qstring.h>
#include <qmap.h>

namespace oxygine
{
    class ResourcesLoadOptions
    {
    public:
        ResourcesLoadOptions() : _loadCompletely(true), _useLoadCounter(false), _shortenIDS(false) {};

        //load only Resources definitions. Skips internal heavy data (atlasses/textures/buffers). Could be overridden in xml: <your_res_type ... load = "false"/>
        ResourcesLoadOptions& dontLoadAll(bool v = false) { _loadCompletely = v; return *this; }

        //use load counter internally
        ResourcesLoadOptions& useLoadCounter(bool v = true) { _useLoadCounter = v; return *this; }

        //use not standard folder with prebuilt resources (atlasses, fonts, etc)
        ResourcesLoadOptions& prebuiltFolder(QString folder) {_prebuilFolder = folder; return *this; }

        //use load counter internally
        ResourcesLoadOptions& shortenIDS(bool v = true) { _shortenIDS = v; return *this; }


        bool _loadCompletely;
        bool _useLoadCounter;
        bool _shortenIDS;
        QString _prebuilFolder;
    };

    class Resources: public Resource
    {
    public:
        typedef QVector<spResource> resources;
        typedef QMap<QString, spResource> resourcesMap;

        typedef Resource* (*createResourceCallback)(CreateResourceContext& context);

        /**Registers your own resource type.
        @param creation callback
        @param resource type string identifier. Max id length is 15 chars. These IDs are already occupied: 'set', 'atlas', ' image', 'font', 'buffer'
        */
        static void registerResourceType(createResourceCallback creationCallback, QString resTypeID);
        static void unregisterResourceType(QString resTypeID);
        static void setDefaultMissingResAnim(ResAnim*);

        Resources();
        ~Resources();

        /**Loads resources from xml file. Load could be called multiple times for different xml files.
        @param xml file paths
        @param options
        */
        bool loadXML(const QString xmlFile, const ResourcesLoadOptions& opt = ResourcesLoadOptions());

        /**Adds Resource*/
        void add(Resource* r, bool accessByShortenID = false);

        /**Calls Resource::load for each resource in the list*/
        void load();

        /**Unloads data from memory, all resources handles remain valid*/
        void unload();

        /**Completely deletes all loaded resources*/
        void free();

        /**Does it have any loaded xmls*/
        bool isEmpty() const;

        /** get resource by id, no case sensitive
        @param resource id
        */
        Resource* get(QString id, error_policy ep = ep_show_error, Resource* defIfNotFound = 0) const;

        /** get resource by id
        @param resource id
        */
        virtual ResAnim* getResAnim(QString id, error_policy ep = ep_show_error) const;

        /** get animation resource by id
        @param resource id
        */
        ResFont* getResFont(QString id, error_policy ep = ep_show_error) const;

        template<class T>
        T* getT(QString id, error_policy ep = ep_show_error, T* defIfNotFound = 0) const { return safeCast<T*>(get(id, ep, defIfNotFound)); }

        /**debug function. prints all loaded resources*/
        void print() const;

        /**collects all resources into vector*/
        void collect(resources&);

        resources& _getResources();
        resourcesMap& _getResourcesMap();

        void setLinearFilter(quint32 linearFilter);
        quint32 getLinearFilter() const;
    protected:
        void updateName(QString filename);
        void _load(LoadResourcesContext* context) override;
        void _unload() override;


        struct registeredResource
        {
            registeredResource() {id;}
            QString id;
            createResourceCallback cb;

            static bool comparePred2(const registeredResource& ob1, QString ob)
            {
                return ob1.id > ob;
            }

            static bool comparePred(const registeredResource& ob1, const registeredResource& ob2)
            {
                return ob1.id > ob2.id;
            }

            bool operator < (QString ob2) const
            {
                return this->id > ob2;
            }
        };


        resources _resources;
        resourcesMap _resourcesMap;


        typedef QVector< registeredResource > registeredResources;
        static registeredResources _registeredResources;

        QString _name;
        QVector<QDomDocument> _docs;
    };
}
