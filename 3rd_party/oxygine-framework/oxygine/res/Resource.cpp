#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "qfileinfo.h"

namespace oxygine
{
    Resource::Resource()
        : m_parent(0),
          m_loadCounter(0),
          m_useLoadCounter(false)
    {

    }

    Resource::~Resource()
    {

    }

    void Resource::load(LoadResourcesContext* context /* = 0 */)
    {
        if (!context)
        {
            context = LoadResourcesContext::get();
        }

        if (m_loadCounter == 0)
        {
            _load(context);
        }

        m_loadCounter = m_useLoadCounter ? m_loadCounter + 1 : 1;
    }

    void Resource::unload()
    {
        if (m_loadCounter == 1)
        {
            _unload();
        }
        m_loadCounter = m_useLoadCounter ? m_loadCounter - 1 : 0;
    }

    QString Resource::extractID(const QDomElement& node, QString file, QString def)
    {
        QString id = node.attribute("id");
        if (id.isEmpty())
        {
            if (file.isEmpty())
            {
                return def.toLower();
            }
            return extractID(file);
        }

        return id.toLower();
    }

    QString Resource::extractID(QString file)
    {
        QFileInfo info(file);
        return info.baseName().toLower();
    }


    QString Resource::getAttribute(QString attr) const
    {
        return m_node.attribute(attr);
    }

    QString Resource::getName() const
    {
        return m_name;
    }

    void Resource::setName(QString name)
    {
        m_name = name;
    }
}
