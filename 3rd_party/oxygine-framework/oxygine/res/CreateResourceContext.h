#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include <QDomDocument>

namespace oxygine
{
    /**internal class*/
    class XmlWalker final
    {
    public:
        explicit XmlWalker(const QString & path,
                           float scaleFactor,
                           const QDomElement & xml);
       virtual ~XmlWalker() = default;
        bool empty() const
        {
            return m_root.isNull();
        }
        const QString & getCurrentFolder() const
        {
            return m_path;
        }
        QString getPath(const QString & attrName) const;
        const QDomElement & getNode() const
        {
            return m_root;
        }
        float getScaleFactor() const
        {
            return m_scaleFactor;
        }
        QString getType() const
        {
            return m_root.nodeName();
        }
        void checkSetAttributes();
        XmlWalker next();

    private:
        void _checkSetAttributes(const QDomElement & node);

    private:
        QString m_path;
        QDomElement m_root;
        QDomElement m_last;
        bool m_notStarted;
        float m_scaleFactor;
    };

    class CreateResourceContext final
    {
    public:
        explicit CreateResourceContext() = default;
       virtual ~CreateResourceContext() = default;
        Resources* m_resources{nullptr};
        QString m_xml_name;
        XmlWalker m_walker{"", 1.0f, QDomElement()};
    };
}
