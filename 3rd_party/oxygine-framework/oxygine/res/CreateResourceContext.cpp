#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include <qvariant.h>

namespace oxygine
{
    LoadResourcesContext* LoadResourcesContext::get()
    {
        LoadResourcesContext* scontext = &SingleThreadResourcesContext::instance;
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
        : _path(path),
          _root(xml),
          _notStarted(true),
          _scaleFactor(scaleFactor),          
          _load(load),
          _alphaHitTest(alpha)
    {
    }


    QString XmlWalker::getPath(QString attrName) const
    {
        QString str = _root.attribute(attrName);
        return _path + str;
    }

    XmlWalker XmlWalker::next()
    {
        while (1)
        {
            QDomNode node;
            if (_notStarted)
            {
                node = _root.firstChild();
                _notStarted = false;
            }
            else
            {
                node = _last.nextSibling();
            }
            // skip comment nodes for the greater good
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            _last = node.toElement();

            QString name = _last.nodeName();
            if (name == "set")
            {
                _checkSetAttributes(_last);
                continue;
            }


            break;
        }

        return XmlWalker(_path, _scaleFactor, _load, _alphaHitTest, _last);
    }

    void XmlWalker::_checkSetAttributes(QDomElement node)
    {
        auto attr = node.attributes();
        for (qint32 i = 0; i < attr.size(); i++)
        {
            QDomNode node = attr.item(i);
            if (node.nodeName() == "path")
            {
                _path = node.nodeValue();
                if (!_path.isEmpty())
                {
                    _path += "/";
                }
            }
            else if (node.nodeName() ==  "load")
            {
                _load = QVariant(node.nodeValue()).toBool();
            }
            else if (node.nodeName() ==  "scale_factor")
            {
                bool ok = false;
                _scaleFactor = node.nodeValue().toFloat(&ok);
                if (!ok)
                {
                    handleErrorPolicy(error_policy::ep_show_error, "invalid scale factor found in xml");
                    _scaleFactor = 1.0f;
                }
            }
            else if (node.nodeName() ==  "hit_test")
            {
                _alphaHitTest = QVariant(node.nodeValue()).toBool();
            }
        }
    }

    void XmlWalker::checkSetAttributes()
    {
        _checkSetAttributes(_root);
    }

    RestoreResourcesContext RestoreResourcesContext::instance;
    void RestoreResourcesContext::createTexture(const CreateTextureTask& opt)
    {
        opt.dest->init(opt.src->lock(), false);
        opt.ready();
    }

    bool RestoreResourcesContext::isNeedProceed(spNativeTexture)
    {
        return true;
    }

    SingleThreadResourcesContext SingleThreadResourcesContext::instance;

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
