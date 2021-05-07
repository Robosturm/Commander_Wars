#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "QDomDocument"

namespace oxygine
{
    DECLARE_SMART(Resource, spResource);
    class Resource: public Object
    {
    public:
        Resource();
        ~Resource();

        /**Loads resource heavy data into memory*/
        void load(LoadResourcesContext* context = 0);
        /**Unloads heavy data from memory.
        All internal Objects (fonts, textures, sounds etc) remain valid but not usable (can't be displayed, played etc).
        */
        void unload();

        /**Returns any attribute from xml resource definition. You could use it for defining for example per animation constants like duration, specific offsets, delays.*/
        QString             getAttribute(QString attr) const;
        QDomElement         getNode() const {return m_node;}
        bool                getUseLoadCounter() const {return m_useLoadCounter;}
        qint32                 getLoadCounter() const { return m_loadCounter; }
        Resource*           getParent() const {return m_parent;}


        void setUseLoadCounter(bool v) {m_useLoadCounter = v;}
        void setParent(Resource* p) {m_parent = p;}

        /**returns id from xml node. Function is helper*/
        static QString extractID(const QDomElement& node, QString file, QString def);
        static QString extractID(QString file);

        virtual void setLinearFilter(quint32)
        {
        }
        virtual quint32 getLinearFilter() const
        {
            return 0;
        }
        /**
         * @brief getName
         * @return
         */
        QString  getName() const;
        /**
         * @brief setName
         * @param name
         */
        void setName(QString name);
    protected:
        static void setNode(spResource res, const QDomElement& node) {res->m_node = node;}
        virtual void _load(LoadResourcesContext* context = 0) = 0;
        virtual void _unload() = 0;

    protected:
        Resource* m_parent;
        qint32 m_loadCounter;
        bool m_useLoadCounter;
        QDomElement m_node;
        QString m_name;

    private:
        //non copyable
        Resource(const Resource&);
        const Resource& operator=(const Resource&);
    };
}
