#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/core/texture.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include <qvariant.h>

namespace oxygine
{
    XmlWalker::XmlWalker(const QString & path, float scaleFactor, const QDomElement & xml)
        : m_path(path),
          m_root(xml),
          m_notStarted(true),
          m_scaleFactor(scaleFactor)
    {
    }

    QString XmlWalker::getPath(const QString & attrName) const
    {
        QString str = m_root.attribute(attrName);
        return m_path + str;
    }

    XmlWalker XmlWalker::next()
    {
        while (true)
        {
            QDomNode node;
            if (m_notStarted)
            {
                node = m_root.firstChild();
                m_notStarted = false;
            }
            else
            {
                node = m_last.nextSibling();
            }
            // skip comment nodes for the greater good
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            m_last = node.toElement();

            QString name = m_last.nodeName();
            if (name == "set")
            {
                _checkSetAttributes(m_last);
                continue;
            }


            break;
        }

        return XmlWalker(m_path, m_scaleFactor, m_last);
    }

    void XmlWalker::_checkSetAttributes(const QDomElement & node)
    {
        auto attr = node.attributes();
        for (qint32 i = 0; i < attr.size(); i++)
        {
            QDomNode node = attr.item(i);
            if (node.nodeName() == "path")
            {
                m_path = node.nodeValue();
                if (!m_path.isEmpty())
                {
                    m_path += "/";
                }
            }
            else if (node.nodeName() ==  "scale_factor")
            {
                bool ok = false;
                m_scaleFactor = node.nodeValue().toFloat(&ok);
                if (!ok)
                {
                    handleErrorPolicy(error_policy::ep_show_error, "invalid scale factor found in xml");
                    m_scaleFactor = 1.0f;
                }
            }
        }
    }

    void XmlWalker::checkSetAttributes()
    {
        _checkSetAttributes(m_root);
    }
}
