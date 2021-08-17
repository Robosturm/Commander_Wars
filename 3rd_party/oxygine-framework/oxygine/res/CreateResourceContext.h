#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include <QDomDocument>

namespace oxygine
{
    /**internal class*/
    class XmlWalker
    {
    public:
        explicit XmlWalker(QString path,
                           float scaleFactor,
                           bool alpha,
                           QDomElement xml);
        virtual ~XmlWalker() = default;
        bool empty() const
        {
            return m_root.isNull();
        }
        QString  getCurrentFolder() const
        {
            return m_path;
        }
        QString     getPath(QString attrName) const;
        QDomElement getNode() const
        {
            return m_root;
        }
        float getScaleFactor() const
        {
            return m_scaleFactor;
        }
        bool getAlphaHitTest() const
        {
            return m_alphaHitTest;
        }
        QString getType() const
        {
            return m_root.nodeName();
        }
        void checkSetAttributes();
        XmlWalker next();

    private:
        void _checkSetAttributes(QDomElement node);

    private:
        QString m_path;
        QDomElement m_root;
        QDomElement m_last;
        bool m_notStarted;
        float m_scaleFactor;
        bool m_alphaHitTest;
    };

    class CreateResourceContext
    {
    public:
        explicit CreateResourceContext() = default;
        virtual ~CreateResourceContext() = default;
        Resources* m_resources{nullptr};
        QString m_xml_name;
        bool m_addTransparentBorder{false};
        XmlWalker m_walker{"", 1.0f, false, QDomElement()};
    };
}
