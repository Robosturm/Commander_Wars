#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/closure.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include <QDomDocument>
#include <QString>
#include <QMap>
#include <vector>

namespace oxygine
{
    class Resources : public Resource
    {
    public:
        using resources = std::vector<spResource>;
        using resourcesMap = QMap<QString, spResource>;
        using createResourceCallback = spResource(*)(CreateResourceContext& context);

        /**Registers your own resource type.
        @param creation callback
        @param resource type string identifier. Max id length is 15 chars. These IDs are already occupied: 'set', 'atlas', ' image', 'font', 'buffer'
        */
        static void registerResourceType(createResourceCallback creationCallback, const QString & resTypeID);
        static void unregisterResourceType(const QString & resTypeID);

        explicit Resources() = default;
        virtual ~Resources();

        /**Loads resources from xml file. Load could be called multiple times for different xml files.
        @param xml file paths
        @param options
        */
        bool loadXML(const QString & xmlFile, bool addTransparentBorder);

        /**Adds Resource*/
        void add(spResource r);

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
        Resource* get(const QString & id, error_policy ep = ep_ignore_error) const;

        /** get resource by id
        @param resource id
        */
        virtual ResAnim* getResAnim(const QString & id, error_policy ep = ep_ignore_error) const;

        template<class T>
        T* getT(const QString & id, error_policy ep = ep_ignore_error) const
        {
            return safeCast<T*>(get(id, ep));
        }

        resources& _getResources();
        resourcesMap& _getResourcesMap();
    protected:
        void updateName(const QString & filename);
        virtual void _load() override;
        void _unload() override;
        struct registeredResource
        {
            QString id;
            createResourceCallback cb;

            static bool comparePred2(const registeredResource& ob1, QString ob)
            {
                return ob1.id > ob;
            }
            bool operator < (QString ob2) const
            {
                return id > ob2;
            }
        };

    protected:
        resources m_resources;
        resourcesMap m_resourcesMap;
        using registeredResources = QVector<registeredResource>;
        static registeredResources m_registeredResources;
        QString m_name;
        QVector<QDomDocument> m_docs;
    };
}
