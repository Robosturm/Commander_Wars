#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include <qvariant.h>

namespace oxygine
{
    LoadResourcesContext* LoadResourcesContext::get()
    {
        LoadResourcesContext* scontext = &SingleThreadResourcesContext::m_instance;
        return scontext;
    }

    CreateTextureTask::CreateTextureTask(): linearFilter(GL_LINEAR), clamp2edge(true)
    {
    }

    void CreateTextureTask::ready() const
    {
        dest->setLinearFilter(linearFilter);
        dest->setClamp2Edge(clamp2edge);
    }

    XmlWalker::XmlWalker(QString path, float scaleFactor, bool load, bool alpha, QDomElement xml)
        : m_path(path),
          m_root(xml),
          m_notStarted(true),
          m_scaleFactor(scaleFactor),
          m_load(load),
          m_alphaHitTest(alpha)
    {
    }


    QString XmlWalker::getPath(QString attrName) const
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

        return XmlWalker(m_path, m_scaleFactor, m_load, m_alphaHitTest, m_last);
    }

    void XmlWalker::_checkSetAttributes(QDomElement node)
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
            else if (node.nodeName() ==  "load")
            {
                m_load = QVariant(node.nodeValue()).toBool();
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
            else if (node.nodeName() ==  "hit_test")
            {
                m_alphaHitTest = QVariant(node.nodeValue()).toBool();
            }
        }
    }

    void XmlWalker::checkSetAttributes()
    {
        _checkSetAttributes(m_root);
    }

    RestoreResourcesContext RestoreResourcesContext::m_instance;
    void RestoreResourcesContext::createTexture(const CreateTextureTask& opt)
    {
        opt.dest->init(opt.src->lock(), false);
        opt.ready();
    }

    bool RestoreResourcesContext::isNeedProceed(spNativeTexture)
    {
        return true;
    }

    SingleThreadResourcesContext SingleThreadResourcesContext::m_instance;

    void SingleThreadResourcesContext::createTexture(const CreateTextureTask& opt)
    {
        opt.dest->init(opt.src->lock(), false);
        opt.ready();
    }

    bool SingleThreadResourcesContext::isNeedProceed(spNativeTexture t)
    {
        return t->getHandle() == 0;
    }
}
