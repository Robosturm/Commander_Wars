#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "QDomDocument"

namespace oxygine
{
    class Resource;
    using spResource = intrusive_ptr<Resource>;
    class Resource: public Object
    {
    public:
        /**
         * @brief QRC_PREFIX for url resolution
         */
        static const char* const QRC_PREFIX;
        /**
         * @brief RCC_PREFIX_PATH general resource file prefix
         */
        static const char* const RCC_PREFIX_PATH;

        explicit Resource() = default;
        virtual ~Resource() = default;

        /**Loads resource heavy data into memory*/
        void load();
        /**Unloads heavy data from memory.
            All internal Objects (fonts, textures, sounds etc) remain valid but not usable (can't be displayed, played etc).
            */
        void unload();

        /**Returns any attribute from xml resource definition. You could use it for defining for example per animation constants like duration, specific offsets, delays.*/
        QString getAttribute(QString attr) const;
        QDomElement getNode() const
        {
            return m_node;
        }
        Resource* getParent() const
        {
            return m_parent;
        }
        void setParent(Resource* p)
        {
            m_parent = p;
        }
        /**returns id from xml node. Function is helper*/
        static QString extractID(const QDomElement& node, QString file, QString def);
        static QString extractID(QString file);
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
        static void setNode(spResource res, const QDomElement& node)
        {
            res->m_node = node;
        }
        virtual void _load() = 0;
        virtual void _unload() = 0;

    protected:
        Resource* m_parent{nullptr};
        QDomElement m_node;
        QString m_name;

    private:
        //non copyable
        Resource(const Resource&);
        const Resource& operator=(const Resource&);
    };
}
